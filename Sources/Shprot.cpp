#include "Shprot.h"

#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>
#include <QNetworkProxy>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QSignalBlocker>
#include <QStandardPaths>
#include <QThread>
#include <QUrl>

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#include <wtsapi32.h>
#elif defined(Q_OS_LINUX)
#include <QtDBus>
#endif

#include "AboutDialog.h"
#include "HttpProxyServer.h"
#include "Logs.h"
#include "LogsDialog.h"
#include "Preferences.h"
#include "PreferencesDialog.h"
#include "SingleInstance.h"
#include "Translations.h"
#include "TweakStyle.h"
#include "UpdateChecker.h"
#include "Utilities.h"
#include "Websites.h"

namespace {

static const int StartDelayTimeout = 5000; // 5s

static const int TunnelProcessStartTimeout = 2000; // 2s
static const int TunnelProcessWarmupTime = 5000; // 5s since ssh opened the local port
static const int TunnelProcessRestartDelayLazy = 1000; // 1s

// Restart delay doubles after each failed attempt until the tunnel passes a health check
static const int TunnelProcessRestartDelayBase = 1000; // 1s
static const int TunnelProcessRestartDelayMax = 60000; // 1m

// Authentication failures are not going to fix themselves, so retry rarely to avoid being banned
// by tools like fail2ban (default policy is 5 failures within 10 minutes)
static const int TunnelProcessRestartDelayAuthBase = 60000; // 1m
static const int TunnelProcessRestartDelayAuthMax = 1800000; // 30m

// Installed OpenSSH client is picked up without restarting the application
static const int TunnelProcessRestartDelaySshMissing = 60000; // 1m

// Shorter delays would only flash in the menu
static const int TunnelProcessRestartCountdownDelayMin = 5000; // 5s

static const int HealthCheckStartDelay = 100; // 100ms
static const int HealthCheckDelayLong = 20000; // 20s
static const int HealthCheckDelayShort = 50; // 50ms
static const int HealthCheckConnectionTimeout = 8000; // 8s, outlasts the 5s DNS retry of the resolver on the server
static const int HealthCheckFailsLimit = 3;
static const int HealthCheckEarlyIntervalMin = 5000; // 5s, a page with many unreachable resources runs a single check

static const int StatusUpdateTimeout = 500; // 500ms

static const int WakeCheckInterval = 5000; // 5s
static const int WakeTimeGapMin = 30000; // 30s, the time missed by the check that means the system was sleeping

// Tray of the desktop may appear a bit later than the application started at login
static const int MissingTrayCheckDelay = 15000; // 15s

QStringList getAllLoggedInUserNames()
{
#if defined(Q_OS_WINDOWS)

    PWTS_SESSION_INFO sessionsData = NULL;
    DWORD sessionsCount = 0;

    if (!WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &sessionsData, &sessionsCount))
    {
        return QStringList();
    }

    QSet<QString> userNamesSet;

    for (DWORD i = 0; i < sessionsCount; i++)
    {
        LPWSTR userNameData = NULL;
        DWORD userNameSize = 0;

        if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, sessionsData[i].SessionId, WTSUserName,
                                       &userNameData, &userNameSize))
        {
            if (userNameData && (userNameSize > 0))
            {
                QString userName = QString::fromUtf16((const ushort*)userNameData);

                if (!userName.isEmpty() && (userName != "SYSTEM") && (userName != "LOCAL SERVICE") &&
                    (userName != "NETWORK SERVICE"))
                {
                    userNamesSet.insert(userName);
                }
            }

            WTSFreeMemory(userNameData);
        }
    }

    WTSFreeMemory(sessionsData);
    return userNamesSet.values();

#elif defined(Q_OS_LINUX)

    if (!QDBusConnection::systemBus().isConnected())
    {
        qWarning() << "D-Bus system bus is not available.";
        return QStringList();
    }

    QDBusInterface managerInterface("org.freedesktop.login1", "/org/freedesktop/login1",
                                    "org.freedesktop.login1.Manager", QDBusConnection::systemBus());

    if (!managerInterface.isValid())
    {
        qWarning() << "Failed to connect to systemd-logind interface:"
                   << managerInterface.lastError().message();
        return QStringList();
    }

    QDBusMessage reply = managerInterface.call("ListUsers");

    if (reply.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "Error calling ListUsers method:" << reply.errorMessage();
        return QStringList();
    }

    QSet<QString> userNamesSet;

    if (!reply.arguments().isEmpty())
    {
        const QDBusArgument arg = reply.arguments().at(0).value<QDBusArgument>();

        if (arg.currentType() == QDBusArgument::ArrayType)
        {
            arg.beginArray();

            while (!arg.atEnd())
            {
                arg.beginStructure();

                uint32_t uid;
                QString username;
                QDBusObjectPath path;

                arg >> uid >> username >> path;

                if ((uid >= 1000) && !username.isEmpty())
                {
                    userNamesSet.insert(username);
                }

                arg.endStructure();
            }

            arg.endArray();
        }
    }

    return userNamesSet.values();
#else
    return QStringList();
#endif
}

} // namespace

Shprot::Shprot(Preferences* preferences, QObject* parent) : QObject(parent), m_preferences(preferences)
{
    QDir appConfigDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    appConfigDir.mkpath(".");

    connect(m_preferences, &Preferences::changed, this, &Shprot::handlePreferencesChange);

    m_identityFilePath = appConfigDir.filePath("Identity");

    m_tunnelTimer = new QTimer(this);
    m_tunnelTimer->setSingleShot(true);

    connect(m_tunnelTimer, &QTimer::timeout, this, &Shprot::maybeRestartTunnelProcess);

    m_wakeCheckTimer = new QTimer(this);
    m_wakeCheckTimer->setInterval(WakeCheckInterval);

    connect(m_wakeCheckTimer, &QTimer::timeout, this, &Shprot::checkForWake);

    m_tunnelProcess = new QProcess(this);
    connect(m_tunnelProcess, &QProcess::started, this, &Shprot::handleTunnelProcessStart);
    connect(m_tunnelProcess, &QProcess::finished, this, &Shprot::handleTunnelProcessFinish);

    m_healthCheckTimer = new QTimer(this);
    m_healthCheckTimer->setSingleShot(true);

    connect(m_healthCheckTimer, &QTimer::timeout, this, &Shprot::maybeRunHealthCheck);

    for (const QString& website : Websites::Popular)
    {
        m_healthCheckUrls.append(website);
    }

    m_httpProxyServer = new HttpProxyServer(this);
    connect(m_httpProxyServer, &HttpProxyServer::targetConnectionFailed, this, &Shprot::maybeRunEarlyHealthCheck);

    m_contextMenu = new QMenu();

    // Tunnel: status (some trays don't show tool tips), switch and proxy addresses available when it's open

    m_statusAction = m_contextMenu->addAction(QString());
    m_statusAction->setEnabled(false);

    // Failures are still counted, so this doesn't undo the growing delay protecting from being banned
    m_reconnectAction = m_contextMenu->addAction(tr("Reconnect Now"), this, &Shprot::maybeRestartTunnelProcess);
    m_reconnectAction->setVisible(false);

    m_tunnelEnabledAction = m_contextMenu->addAction(tr("Enable SSH Proxy Tunnel"));
    m_tunnelEnabledAction->setCheckable(true);
    m_tunnelEnabledAction->setChecked(m_preferences->value("sshProxyTunnel").toMap().value("enabled", false).toBool());

    connect(m_tunnelEnabledAction, &QAction::toggled, this, &Shprot::setTunnelEnabled);

    m_copySocks5ProxyUrlAction = m_contextMenu->addAction(QString(), this, [this]() {
        QGuiApplication::clipboard()->setText(m_socks5ProxyUrl);
    });

    m_copyHttpProxyUrlAction = m_contextMenu->addAction(QString(), this, [this]() {
        QGuiApplication::clipboard()->setText(m_httpProxyUrl);
    });

    // Application settings

    m_contextMenu->addSeparator();

    m_contextMenu->setDefaultAction(m_contextMenu->addAction(tr("Preferences…"), this, &Shprot::openPreferencesDialog));

    QAction* showNotificationsAction = m_contextMenu->addAction(tr("Show Notifications"));
    showNotificationsAction->setCheckable(true);
    showNotificationsAction->setChecked(m_preferences->value("showNotifications", true).toBool());

    connect(showNotificationsAction, &QAction::toggled, this, [this](bool checked) {
        m_preferences->setValue("showNotifications", checked);
    });

    // Autostart entry is always installed, the application started by it exits if this is turned off
    QAction* startAtLoginAction = m_contextMenu->addAction(tr("Start at Login"));
    startAtLoginAction->setCheckable(true);
    startAtLoginAction->setChecked(m_preferences->value("startAtLogin", true).toBool());

    connect(startAtLoginAction, &QAction::toggled, this, [this](bool checked) {
        m_preferences->setValue("startAtLogin", checked);
    });

    m_contextMenu->addSeparator();

    m_contextMenu->addAction(tr("Show Logs…"), this, &Shprot::openLogsDialog);
    m_contextMenu->addAction(tr("About %1…").arg(PROJECT_TITLE), this, &Shprot::openAboutDialog);

    // Hidden until a newer release is found, the user is not bothered otherwise
    m_newVersionAction = m_contextMenu->addAction(QString(), this, [this]() {
        QDesktopServices::openUrl(m_updateChecker->newVersionUrl());
    });
    m_newVersionAction->setVisible(false);

    m_contextMenu->addSeparator();

    m_contextMenu->addAction(tr("Quit %1").arg(PROJECT_TITLE), qApp, &QApplication::quit);

    m_tunnelOpenIcon = QIcon(":/Shprot.svg");
    m_tunnelClosedIcon = QIcon(":/Inactive.svg");

    m_systemTrayIcon = new QSystemTrayIcon(this);
    m_systemTrayIcon->setIcon(m_tunnelClosedIcon);
    m_systemTrayIcon->setToolTip(PROJECT_TITLE);
    m_systemTrayIcon->setContextMenu(m_contextMenu);
    m_systemTrayIcon->show();

    connect(m_systemTrayIcon, &QSystemTrayIcon::activated, this, &Shprot::handleSystemTrayIconActivation);

    m_statusTimer = new QTimer(this);
    m_statusTimer->setSingleShot(true);

    connect(m_statusTimer, &QTimer::timeout, this, &Shprot::maybeUpdateStatus);

    m_statusCountdownTimer = new QTimer(this);
    m_statusCountdownTimer->setSingleShot(true);

    connect(m_statusCountdownTimer, &QTimer::timeout, this, &Shprot::updateStatusText);

    m_updateChecker = new UpdateChecker(m_preferences, this);
    connect(m_updateChecker, &UpdateChecker::latestVersionChanged, this, &Shprot::updateNewVersionAction);

    updateNewVersionAction(); // Version found before restart is offered right away

    m_preferencesDialog = new PreferencesDialog(m_preferences);

    // The dialog would overwrite the state toggled in the menu when saved, so it's toggled in one place at a time
    connect(m_contextMenu, &QMenu::aboutToShow, this, [this]() {
        m_tunnelEnabledAction->setEnabled(!m_preferencesDialog->isVisible());
    });

    QNetworkInformation* networkInformation = QNetworkInformation::instance();

    connect(networkInformation, &QNetworkInformation::reachabilityChanged,
            this, &Shprot::handleNetworkChange);

    connect(networkInformation, &QNetworkInformation::transportMediumChanged,
            this, &Shprot::handleNetworkChange);

    maybeUpdateStatus();
}

Shprot::~Shprot()
{
    stop();
    delete m_contextMenu;
    delete m_preferencesDialog;
    delete m_logsDialog;
    delete m_aboutDialog;
}

void Shprot::start()
{
    m_tunnelTimer->start(0);
    m_updateChecker->start();

    m_wakeCheckTime = QDateTime::currentMSecsSinceEpoch();
    m_wakeCheckTimer->start();

    QTimer::singleShot(MissingTrayCheckDelay, this, &Shprot::maybeWarnAboutMissingTray);
}

void Shprot::stop()
{
    stopTunnelProcess();
}

void Shprot::openPreferencesDialog()
{
    m_preferencesDialog->open();
}

void Shprot::openLogsDialog()
{
    if (!m_logsDialog)
    {
        m_logsDialog = new LogsDialog();
    }

    m_logsDialog->open();
}

void Shprot::openAboutDialog()
{
    // Created on demand, since license texts take some memory
    if (!m_aboutDialog)
    {
        m_aboutDialog = new AboutDialog();
    }

    m_aboutDialog->open();
}

void Shprot::maybeRestartTunnelProcess()
{
    stopHealthCheck();

    m_tunnelTimer->stop();
    stopTunnelProcess();

    QVariantMap sshProxyTunnel = m_preferences->value("sshProxyTunnel").toMap();
    QNetworkInformation* networkInformation = QNetworkInformation::instance();

    if (!sshProxyTunnel.value("enabled", false).toBool())
    {
        setLastError(QString(), QString());
        return; // SSH Proxy Tunnel is disabled
    }

    // Only a missing network stops the attempt: the system reports a limited connection when its own connectivity
    // check is blocked, and the server may be available anyway (e.g. in the local network)
    if (networkInformation && (networkInformation->reachability() == QNetworkInformation::Reachability::Disconnected))
    {
        QString noNetworkMessage = tr("No network connection");
        setLastError(noNetworkMessage, noNetworkMessage);
        m_notifiedError = noNetworkMessage; // Shown in the status, not a failure to notify about
        return;
    }

    QString missingSshMessage = Utilities::getMissingSshToolMessage("ssh");

    if (!missingSshMessage.isEmpty())
    {
        qWarning() << qPrintable(missingSshMessage);
        setLastError(missingSshMessage, tr("OpenSSH not installed"));
        m_tunnelTimer->start(TunnelProcessRestartDelaySshMissing);
        return;
    }

    QString sshDestination = sshProxyTunnel.value("sshDestination").toString();
    quint16 sshPort = Utilities::parsePort(sshProxyTunnel.value("sshPort").toString(), 22);

    QString sshAddress = Utilities::getAddressFromSshDestination(sshDestination);
    QString knownHostsPath = Utilities::getKnownHostsPath(sshAddress);

    QString localSocks5ProxyHost = sshProxyTunnel.value("localSocks5ProxyHost", "localhost").toString();

    if (localSocks5ProxyHost.isEmpty())
    {
        localSocks5ProxyHost = "localhost";
    }

    quint16 localSocks5ProxyPort = Utilities::parsePort(sshProxyTunnel.value("localSocks5ProxyPort").toString(), 1080);
    QString localSocks5ProxyAddress = QString("%1:%2").arg(localSocks5ProxyHost).arg(localSocks5ProxyPort);

    QString privateKey = sshProxyTunnel.value("sshPrivateKey").toString();

    // ssh can't ask for a passphrase in batch mode, so retrying is useless until the settings are changed
    if (Utilities::sshPrivateKeyIsEncrypted(privateKey))
    {
        qWarning() << "SSH private key is protected with a passphrase";
        setLastError(Utilities::getEncryptedPrivateKeyMessage(), tr("Private key has a passphrase"));
        return;
    }

    if (!Utilities::writePrivateKeyFile(m_identityFilePath, privateKey.toUtf8()))
    {
        qCritical() << "Unable to write identity file";

        QString writeFailedMessage = tr("Unable to write private key file");
        setLastError(writeFailedMessage, writeFailedMessage);
        scheduleTunnelProcessRestart(false);
        return;
    }

    m_tunnelSocksProxy.setType(QNetworkProxy::Socks5Proxy);
    m_tunnelSocksProxy.setHostName(Utilities::getProxyConnectionHost(localSocks5ProxyHost));
    m_tunnelSocksProxy.setPort(localSocks5ProxyPort);
    m_tunnelSocksProxy.setCapabilities(m_tunnelSocksProxy.capabilities() | QNetworkProxy::HostNameLookupCapability);

    // Host names are resolved on the server side ("socks5h"), as Shprot itself does
    m_socks5ProxyUrl = Utilities::formatProxyUrl("socks5h", localSocks5ProxyHost, localSocks5ProxyPort);

    QStringList arguments;
    arguments << "-D" << localSocks5ProxyAddress;
    arguments << "-N" << sshDestination << "-p" << QString::number(sshPort);
    arguments << "-o" << "IdentitiesOnly=yes";
    arguments << "-o" << "IdentityAgent=none";
    arguments << "-o" << "PreferredAuthentications=publickey";
    arguments << "-o" << QString("IdentityFile='%1'").arg(QDir::toNativeSeparators(m_identityFilePath));
    arguments << "-o" << QString("UserKnownHostsFile='%1'").arg(QDir::toNativeSeparators(knownHostsPath));
    arguments << "-o" << "StrictHostKeyChecking=yes";
    arguments << "-o" << "BatchMode=yes";
    arguments << "-o" << "ExitOnForwardFailure=yes";
    arguments << "-o" << "ConnectTimeout=10";
    arguments << "-o" << "ServerAliveInterval=5";
    arguments << "-o" << "ServerAliveCountMax=3";
    arguments << "-o" << "TCPKeepAlive=yes";

#if defined(Q_OS_WIN)
    arguments << "-F" << "NUL";
#elif defined(Q_OS_LINUX)
    arguments << "-F" << "/dev/null";
#endif

    m_tunnelProcess->start("ssh", arguments);
    m_tunnelStoppedIntentionally = false;

    if (!m_tunnelProcess->waitForStarted(TunnelProcessStartTimeout))
    {
        qWarning() << "Failed to start tunnel process:" << m_tunnelProcess->errorString();
        setLastError(tr("Unable to start ssh: %1").arg(m_tunnelProcess->errorString()), tr("Unable to start ssh"));
        m_tunnelTimer->start(TunnelProcessRestartDelaySshMissing);
        return;
    }
}

void Shprot::maybeRestartTunnelProcessLater()
{
    m_tunnelTimer->start(TunnelProcessRestartDelayLazy);
}

void Shprot::stopTunnelProcess(int timeout)
{
    qint64 pid = m_tunnelProcess->processId();

    if ((m_tunnelProcess->state() != QProcess::NotRunning) && (pid > 0))
    {
        m_tunnelStoppedIntentionally = true;

    #if defined(Q_OS_WIN)
        FreeConsole();

        if (AttachConsole(static_cast<DWORD>(pid)))
        {
            SetConsoleCtrlHandler(NULL, TRUE);
            GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);

            if (!m_tunnelProcess->waitForFinished(timeout))
            {
                m_tunnelProcess->kill();
                m_tunnelProcess->waitForFinished();
            }

            SetConsoleCtrlHandler(NULL, FALSE);
            FreeConsole();
        }
        else
        {
            qWarning() << "Unable to attach to tunnel process console, killing it...";
            m_tunnelProcess->kill();
            m_tunnelProcess->waitForFinished();
        }
    #else
        m_tunnelProcess->terminate();

        if (!m_tunnelProcess->waitForFinished(timeout))
        {
            m_tunnelProcess->kill();
            m_tunnelProcess->waitForFinished();
        }
    #endif
    }

    QFile::remove(m_identityFilePath);
}

void Shprot::handleTunnelProcessStart()
{
    m_tunnelPortOpened = false;
    m_tunnelUptimeTimer.invalidate();

    m_healthCheckTimer->start(HealthCheckStartDelay);
    m_healthCheckFailsCount = 0;

    maybeStartHttpProxyServer();

    // Frequent health checks during warmup keep postponing the status timer, connection attempt is shown right away
    updateStatusText();
    m_statusTimer->start(StatusUpdateTimeout);
}

void Shprot::handleTunnelProcessFinish(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_httpProxyServer->stop();
    stopHealthCheck();

    m_internetReachable = false;
    m_statusTimer->start(StatusUpdateTimeout);

    if (m_tunnelStoppedIntentionally)
    {
        return;
    }

    QByteArray errorOutput = m_tunnelProcess->readAllStandardError().trimmed();

    if (exitCode != 0)
    {
        qWarning() << "Tunnel process ended with error:" << exitCode << errorOutput;
    }

    QString errorDescription = Utilities::describeSshError(errorOutput);

    if (!errorDescription.isEmpty())
    {
        setLastError(errorDescription, Utilities::summarizeSshError(errorOutput));
    }

    // "Permission denied (publickey)." – not to be confused with "bind ...: Permission denied"
    bool authenticationFailed = errorOutput.contains("Permission denied (") ||
                                errorOutput.contains("Too many authentication failures") ||
                                errorOutput.contains("Host key verification failed");

    scheduleTunnelProcessRestart(authenticationFailed);
}

void Shprot::scheduleTunnelProcessRestart(bool authenticationFailed)
{
    int baseDelay = authenticationFailed ? TunnelProcessRestartDelayAuthBase : TunnelProcessRestartDelayBase;
    int maxDelay = authenticationFailed ? TunnelProcessRestartDelayAuthMax : TunnelProcessRestartDelayMax;

    qint64 delay = qint64(baseDelay) << qMin(m_failedRestartsCount, 16);
    delay = qMin(delay, qint64(maxDelay));

    m_failedRestartsCount++;

    if (delay >= TunnelProcessRestartDelayMax)
    {
        qWarning() << qPrintable(QString("Tunnel restart #%1 is scheduled in %2s")
                                 .arg(m_failedRestartsCount).arg(delay / 1000));
    }

    m_tunnelTimer->start(static_cast<int>(delay));
}

void Shprot::handleNetworkChange()
{
    m_failedRestartsCount = 0; // New network deserves a fresh start
    setLastError(QString(), QString()); // The reason found for the previous network is out of date
    maybeRestartTunnelProcessLater();
}

// Wall clock is used, since monotonic clock used by timers doesn't count the time of sleep on Linux. Changing the
// system time may cause an extra reconnection, which is harmless.
void Shprot::checkForWake()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 missedTime = currentTime - m_wakeCheckTime - WakeCheckInterval;

    m_wakeCheckTime = currentTime;

    if (missedTime >= WakeTimeGapMin)
    {
        qInfo() << qPrintable(QString("System woke up after %1s, reconnecting...").arg(missedTime / 1000));
        handleNetworkChange(); // Connection most likely didn't survive, so don't wait until it's found to be stalled
    }
}

void Shprot::maybeWarnAboutMissingTray()
{
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        return;
    }

    qWarning() << "System tray is not available";

    // Shown once, otherwise it would appear at every login
    if (m_preferences->value("missingTrayWarningShown", false).toBool())
    {
        return;
    }

    m_preferences->setValue("missingTrayWarningShown", true);

    QMessageBox* messageBox = new QMessageBox();
    messageBox->setAttribute(Qt::WA_DeleteOnClose);
    messageBox->setWindowTitle(tr("System Tray Not Available"));
    messageBox->setIcon(QMessageBox::Warning);
    messageBox->setText(tr("%1 keeps working in the background, but its icon, menu and notifications can't be shown, "
                           "since this desktop has no system tray. To open the preferences, start %1 again. "
                           "On GNOME, the tray is provided by the AppIndicator extension.").arg(PROJECT_TITLE));
    messageBox->show();
}

void Shprot::stopHealthCheck()
{
    m_healthCheckTimer->stop();

    if (m_healthCheckSocket)
    {
        QTcpSocket* socket = m_healthCheckSocket;
        m_healthCheckSocket = nullptr;
        socket->disconnectFromHost();
    }
}

void Shprot::maybeRunHealthCheck()
{
    stopHealthCheck();

    if ((m_tunnelProcess->state() != QProcess::Running))
    {
        return;
    }

    int healthCheckUrlIndex = QRandomGenerator::global()->bounded(m_healthCheckUrls.count());
    const QUrl& healthCheckUrl = m_healthCheckUrls.value(healthCheckUrlIndex);

    QTcpSocket* healthCheckSocket = new QTcpSocket(this);
    healthCheckSocket->setProperty("url", healthCheckUrl);
    healthCheckSocket->setProperty("portProbe", !m_tunnelPortOpened);
    healthCheckSocket->setProxy(m_tunnelPortOpened ? m_tunnelSocksProxy : QNetworkProxy(QNetworkProxy::NoProxy));

    QTimer::singleShot(HealthCheckConnectionTimeout, healthCheckSocket, [healthCheckSocket]() {
        QAbstractSocket::SocketState state = healthCheckSocket->state();
        if ((state == QAbstractSocket::HostLookupState) || (state == QAbstractSocket::ConnectingState))
        {
            if (!healthCheckSocket->property("portProbe").toBool())
            {
                qWarning() << "Aborting health checker after timeout...";
            }

            healthCheckSocket->abort();
        }
    });

    connect(healthCheckSocket, &QTcpSocket::stateChanged, this, &Shprot::handleHealthCheckSocketStateChange);

    m_healthCheckSocket = healthCheckSocket;

    if (!m_tunnelPortOpened)
    {
        m_healthCheckSocket->connectToHost(m_tunnelSocksProxy.hostName(), m_tunnelSocksProxy.port());
        return;
    }

    m_healthCheckRunTimer.start();

    quint16 healthCheckUrlPort = healthCheckUrl.port(healthCheckUrl.scheme().toLower() == "https" ? 443 : 80);
    m_healthCheckSocket->connectToHost(healthCheckUrl.host(), healthCheckUrlPort);
}

// Failed connection of a client hints that the tunnel may be broken, so it is checked right away instead of
// waiting for the regular check. Only the health check decides on restart, since the website may be down itself.
void Shprot::maybeRunEarlyHealthCheck()
{
    if (!m_tunnelPortOpened || m_healthCheckSocket || (m_tunnelProcess->state() != QProcess::Running))
    {
        return; // Still connecting, or the check is already running
    }

    if (m_healthCheckRunTimer.isValid() && (m_healthCheckRunTimer.elapsed() < HealthCheckEarlyIntervalMin))
    {
        return;
    }

    if (m_healthCheckTimer->isActive() && (m_healthCheckTimer->remainingTime() <= HealthCheckDelayShort))
    {
        return; // Retry after a failed check is coming anyway
    }

    m_healthCheckTimer->start(HealthCheckDelayShort);
}

void Shprot::handleHealthCheckSocketStateChange(QAbstractSocket::SocketState state)
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    if (!socket)
    {
        return;
    }

    switch (state)
    {
    case QAbstractSocket::ConnectedState:
    {
        socket->setProperty("success", true);
        socket->disconnectFromHost();
        break;
    }
    case QAbstractSocket::UnconnectedState:
    {
        socket->deleteLater();

        if (socket != m_healthCheckSocket)
        {
            return; // Obsolete socket
        }

        m_healthCheckSocket = nullptr;

        // ssh is still connecting until its local port is opened, and it gives up on its own (ConnectTimeout,
        // ServerAliveInterval). Failed or aborted probe is harmless, since opened port accepts right away.
        if (socket->property("portProbe").toBool())
        {
            if (socket->property("success").toBool())
            {
                m_tunnelPortOpened = true;
                m_tunnelWarmupTimer.start();
                m_healthCheckTimer->start(HealthCheckDelayShort);
            }
            else
            {
                m_healthCheckTimer->start(HealthCheckStartDelay);
            }

            return;
        }

        if (socket->property("success").toBool())
        {
            m_internetReachable = true;
            m_statusTimer->start(StatusUpdateTimeout);

            // Counted for the ssh process, since a quick reconnection may not even be indicated as closing the tunnel
            if (!m_tunnelUptimeTimer.isValid())
            {
                m_tunnelUptimeTimer.start();
            }

            m_healthCheckFailsCount = 0;
            m_failedRestartsCount = 0;
            m_healthCheckTimer->start(HealthCheckDelayLong);

            m_lastError.clear();
            m_lastErrorSummary.clear();
            m_notifiedError.clear();
        }
        else if (m_tunnelWarmupTimer.elapsed() < TunnelProcessWarmupTime)
        {
            m_internetReachable = false;
            m_statusTimer->start(StatusUpdateTimeout);
            m_healthCheckTimer->start(HealthCheckStartDelay);
        }
        else
        {
            m_internetReachable = false;
            m_statusTimer->start(StatusUpdateTimeout);

            m_healthCheckFailsCount += 1;

            qWarning() << qPrintable(QString("Health check failed (%1/%2) for %3")
                                     .arg(m_healthCheckFailsCount)
                                     .arg(HealthCheckFailsLimit)
                                     .arg(socket->property("url").toUrl().toString()));

            if (m_healthCheckFailsCount < HealthCheckFailsLimit)
            {
                m_healthCheckTimer->start(HealthCheckDelayShort);
            }
            else
            {
                setLastError(tr("Connected to the server, but websites are not reachable through the tunnel"),
                             tr("No internet through tunnel"));
                scheduleTunnelProcessRestart(false);
            }
        }

        break;
    }
    }
}

void Shprot::maybeStartHttpProxyServer()
{
    m_httpProxyServer->stop();
    m_httpProxyUrl.clear();

    if ((m_tunnelProcess->state() != QProcess::Running))
    {
        return;
    }

    QVariantMap sshProxyTunnel = m_preferences->value("sshProxyTunnel").toMap();

    if (!sshProxyTunnel.value("enabled", false).toBool() ||
        !sshProxyTunnel.value("localHttpProxyEnabled", false).toBool())
    {
        m_httpProxyError.clear();
        return; // SSH Proxy Tunnel or Local HTTP Proxy are disabled
    }

    QString localHttpProxyHost = sshProxyTunnel.value("localHttpProxyHost", "localhost").toString();

    if (localHttpProxyHost.isEmpty())
    {
        localHttpProxyHost = "localhost";
    }

    quint16 localHttpProxyPort = Utilities::parsePort(sshProxyTunnel.value("localHttpProxyPort").toString(), 8080);
    QString httpProxyError;

    if (m_httpProxyServer->start(localHttpProxyHost, localHttpProxyPort, m_tunnelSocksProxy))
    {
        m_httpProxyUrl = Utilities::formatProxyUrl("http", localHttpProxyHost, localHttpProxyPort);
    }
    else
    {
        httpProxyError = tr("Local HTTP proxy is not available: %1").arg(m_httpProxyServer->errorString());
    }

    // Notify once per new problem, the tunnel itself keeps working
    if (!httpProxyError.isEmpty() && (httpProxyError != m_httpProxyError) &&
        m_preferences->value("showNotifications", true).toBool())
    {
        m_systemTrayIcon->showMessage(tr("HTTP proxy failed"), httpProxyError, QSystemTrayIcon::NoIcon);
    }

    m_httpProxyError = httpProxyError;
    m_statusTimer->start(StatusUpdateTimeout);
}

void Shprot::handlePreferencesChange(const QStringList& changes)
{
    if (changes.contains("sshProxyTunnel"))
    {
        m_failedRestartsCount = 0;
        m_lastError.clear(); // New settings deserve fresh notifications
        m_lastErrorSummary.clear();
        m_notifiedError.clear();
        m_httpProxyError.clear();
        m_tunnelTimer->start(0);

        QSignalBlocker blocker(m_tunnelEnabledAction);
        m_tunnelEnabledAction->setChecked(m_preferences->value("sshProxyTunnel").toMap().value("enabled", false).toBool());
    }
}

void Shprot::setTunnelEnabled(bool enabled)
{
    QVariantMap sshProxyTunnel = m_preferences->value("sshProxyTunnel").toMap();

    // Tunnel that was never configured can't be enabled from the menu, preferences are opened instead
    if (enabled && (sshProxyTunnel.value("sshDestination").toString().isEmpty() ||
                    sshProxyTunnel.value("sshPrivateKey").toString().isEmpty()))
    {
        QSignalBlocker blocker(m_tunnelEnabledAction);
        m_tunnelEnabledAction->setChecked(false);
        openPreferencesDialog();
        return;
    }

    sshProxyTunnel.insert("enabled", enabled);
    m_preferences->setValue("sshProxyTunnel", sshProxyTunnel);
}

void Shprot::handleSystemTrayIconActivation(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        openPreferencesDialog();
    }
    else if (reason == QSystemTrayIcon::Trigger)
    {
        if (m_systemTrayIconTriggerTimer.isValid() && (m_systemTrayIconTriggerTimer.elapsed() < 500))
        {
            openPreferencesDialog();
        }

        m_systemTrayIconTriggerTimer.start();
    }
}

void Shprot::maybeUpdateStatus()
{
    bool tunnelIsEnabled = m_preferences->value("sshProxyTunnel").toMap().value("enabled", false).toBool();
    bool tunnelIsOpened = (m_tunnelProcess->state() == QProcess::Running) && m_internetReachable;
    bool showNotifications = m_preferences->value("showNotifications", true).toBool();

    updateStatusText();

    // Proxy URLs are offered only while they can be used
    auto updateCopyAction = [tunnelIsOpened](QAction* action, const QString& label, const QString& url) {
        action->setVisible(tunnelIsOpened && !url.isEmpty());
        action->setText(tr("Copy %1 Proxy URL").arg(label));
    };

    updateCopyAction(m_copySocks5ProxyUrlAction, "SOCKS5", m_socks5ProxyUrl);
    updateCopyAction(m_copyHttpProxyUrlAction, "HTTP", m_httpProxyUrl);

    if (m_tunnelIndicatedAsOpened != tunnelIsOpened)
    {
        m_tunnelIndicatedAsOpened = tunnelIsOpened;

        // Updates are checked through the tunnel while it's open, the same way the user goes online
        m_updateChecker->setProxy(tunnelIsOpened ? m_tunnelSocksProxy : QNetworkProxy(QNetworkProxy::NoProxy));

        if (tunnelIsOpened)
        {
            m_systemTrayIcon->setIcon(m_tunnelOpenIcon);

            if (showNotifications)
            {
                m_systemTrayIcon->showMessage(tr("Tunnel opened"), tr("You can use your proxy"), QSystemTrayIcon::NoIcon);
            }
        }
        else
        {
            m_systemTrayIcon->setIcon(m_tunnelClosedIcon);
            m_notifiedError = m_lastError;

            if (showNotifications)
            {
                QString message = m_lastError.isEmpty() ? tr("Proxy is not available") : m_lastError;
                m_systemTrayIcon->showMessage(tr("Tunnel closed"), message, QSystemTrayIcon::NoIcon);
            }
        }

        return;
    }

    // The tunnel may fail without ever being opened, so report each new reason once, not every retry
    if (!tunnelIsOpened && !m_lastError.isEmpty() && (m_lastError != m_notifiedError))
    {
        m_notifiedError = m_lastError;

        if (showNotifications)
        {
            m_systemTrayIcon->showMessage(tr("Unable to open tunnel"), m_lastError, QSystemTrayIcon::NoIcon);
        }
    }
}

void Shprot::updateStatusText()
{
    bool tunnelIsEnabled = m_preferences->value("sshProxyTunnel").toMap().value("enabled", false).toBool();
    bool tunnelIsOpened = (m_tunnelProcess->state() == QProcess::Running) && m_internetReachable;

    bool restartIsAwaited = tunnelIsEnabled && !tunnelIsOpened && m_tunnelTimer->isActive() &&
                            (m_tunnelTimer->interval() >= TunnelProcessRestartCountdownDelayMin);

    bool tunnelIsConnecting = tunnelIsEnabled && !tunnelIsOpened && !restartIsAwaited &&
                              (m_tunnelProcess->state() != QProcess::NotRunning);

    QString status;
    QString menuStatus; // Only differs from the status when there is an error to explain
    QString statusDetails; // Uptime or what happens next, always stays visible when the status is shortened

    if (!tunnelIsEnabled)
    {
        status = tr("Tunnel is disabled");
    }
    else if (tunnelIsOpened)
    {
        status = tr("Tunnel is open");

        // Refreshed by every health check, so minutes are accurate enough
        QString uptime = m_tunnelUptimeTimer.isValid() ? Utilities::formatUptime(m_tunnelUptimeTimer.elapsed()) : QString();

        if (!uptime.isEmpty())
        {
            statusDetails = QString(" · %1").arg(uptime);
        }
    }
    else if (tunnelIsConnecting && m_lastError.isEmpty())
    {
        status = tr("Connecting…");
    }
    else
    {
        status = m_lastError.isEmpty() ? tr("Tunnel is closed") : tr("Tunnel is closed: %1").arg(m_lastError);
        menuStatus = m_lastErrorSummary;

        if (tunnelIsConnecting)
        {
            statusDetails = QString(" (%1)").arg(tr("connecting…"));
        }
    }

    if (restartIsAwaited)
    {
        int remainingTime = m_tunnelTimer->remainingTime();
        int remainingSeconds = (remainingTime + 999) / 1000;

        QString countdown = QString("%1:%2").arg(remainingSeconds / 60).arg(remainingSeconds % 60, 2, 10, QChar('0'));
        statusDetails = QString(" (%1)").arg(tr("retry in %1").arg(countdown));

        // Woken up exactly when the displayed second changes
        m_statusCountdownTimer->start(remainingTime % 1000 + 1);
    }

    m_reconnectAction->setVisible(restartIsAwaited);

    if (menuStatus.isEmpty())
    {
        menuStatus = status;
    }

    // Translations may turn out long and would make the menu too wide
    static const int MenuStatusLengthMax = 64;
    int menuStatusLength = MenuStatusLengthMax - statusDetails.size();
    m_statusAction->setText(((menuStatus.size() > menuStatusLength) ? menuStatus.left(menuStatusLength - 1) + "…" : menuStatus) + statusDetails);

    QString toolTip = status + statusDetails;

    if (!m_httpProxyError.isEmpty())
    {
        toolTip += "\n" + m_httpProxyError;
    }

    m_systemTrayIcon->setToolTip(QString("%1\n%2").arg(PROJECT_TITLE, toolTip));
}

void Shprot::updateNewVersionAction()
{
    QString newVersion = m_updateChecker->newVersion();

    m_newVersionAction->setText(tr("New Version Available (%1 → %2)").arg(PROJECT_VERSION, newVersion));
    m_newVersionAction->setVisible(!newVersion.isEmpty());
}

void Shprot::setLastError(const QString& error, const QString& errorSummary)
{
    m_lastError = error;
    m_lastErrorSummary = errorSummary;
    m_statusTimer->start(StatusUpdateTimeout);
}

// Main function

int main(int argc, char* argv[])
{
#if defined(Q_OS_LINUX)
    qputenv("QT_QPA_PLATFORM", "xcb");
#endif

    QCoreApplication::setApplicationName(PROJECT_NAME);

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir appDataDir(appDataPath);
    appDataDir.mkpath(".");

    QString logsDirPath = appDataDir.filePath("Logs");
    Logs::init(logsDirPath);

    QStringList arguments;

    for (int i = 0; i < argc; i++)
    {
        arguments.append(QString::fromLocal8Bit(argv[i]));
    }

    if (arguments.contains("--shutdown"))
    {
        for (const QString& userName : getAllLoggedInUserNames())
        {
            SingleInstance::sendMessage(SingleInstance::nameForUser(userName), "quit");
        }

        return EXIT_SUCCESS;
    }

    // Preferences are loaded before anything else, so a disabled autostart exits right away
    QDir appConfigDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    appConfigDir.mkpath(".");

    Preferences preferences(appConfigDir.filePath("Preferences.json"));

    if (arguments.contains("--autostart") && !preferences.value("startAtLogin", true).toBool())
    {
        return EXIT_SUCCESS; // Started by the autostart entry, but the user turned it off
    }

    if (arguments.contains("--delay"))
    {
        QThread::msleep(StartDelayTimeout);
    }

    QApplication application(argc, argv);
    Translations::install();

    // Check for other instances already running, ask the running one to show up

    SingleInstance singleInstance(SingleInstance::nameForUser(Utilities::getCurrentUserName()), SingleInstance::defaultLockDirectory());

    if (!singleInstance.start())
    {
        return EXIT_FAILURE;
    }

    if (!QNetworkInformation::loadDefaultBackend())
    {
        qWarning() << "Unable to load default network backend.";
    }

    QApplication::setWindowIcon(QIcon(":/Shprot.svg"));
    QApplication::setStyle(new TweakStyle(application.style()));
    QApplication::setQuitOnLastWindowClosed(false);

    Shprot shprot(&preferences);
    shprot.start();

    QObject::connect(&singleInstance, &SingleInstance::activationRequested, &shprot, &Shprot::openPreferencesDialog);
    QObject::connect(&singleInstance, &SingleInstance::quitRequested, &application, &QCoreApplication::quit);

    if (arguments.contains("--preferences"))
    {
        shprot.openPreferencesDialog();
    }

    return application.exec();
}
