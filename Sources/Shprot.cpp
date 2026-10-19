#include "Shprot.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMessageBox>
#include <QNetworkProxy>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QThread>
#include <QUrl>

#include <memory>

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#include <wtsapi32.h>
#elif defined(Q_OS_LINUX)
#include <QtDBus>
#endif

#include "HttpProxyServer.h"
#include "Logs.h"
#include "Preferences.h"
#include "PreferencesDialog.h"
#include "TweakStyle.h"
#include "Utilities.h"
#include "Websites.h"

namespace {

static const int StartDelayTimeout = 5000; // 5s

static const int SingleInstanceWaitTimeout = 500; // 500ms
static const int SingleInstanceReadTimeout = 5000; // 5s
static const int SingleInstanceMessageSizeMax = 64;
static const int SingleInstanceWriteTimeout = 5000; // 5s

static const int TunnelProcessStartTimeout = 2000; // 2s
static const int TunnelProcessWarmupTime = 5000; // 5s
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

static const int HealthCheckStartDelay = 100; // 100ms
static const int HealthCheckDelayLong = 20000; // 20s
static const int HealthCheckDelayShort = 50; // 50ms
static const int HealthCheckConnectionTimeout = 3000; // 3s
static const int HealthCheckFailsLimit = 3;

static const int StatusUpdateTimeout = 500; // 500ms

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

QString getSingleInstanceServerName(const QString& userName)
{
    return QString("%1:%2").arg(PROJECT_NAME).arg(userName);
}

} // namespace

Shprot::Shprot(QObject* parent) : QObject(parent)
{
    QString appConfigPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    QDir appConfigDir(appConfigPath);
    appConfigDir.mkpath(".");

    QString preferencesPath = appConfigDir.filePath("Preferences.json");
    m_preferences = new Preferences(preferencesPath, this);

    connect(m_preferences, &Preferences::changed, this, &Shprot::handlePreferencesChange);

    m_identityFilePath = appConfigDir.filePath("Identity");

    m_tunnelTimer = new QTimer(this);
    m_tunnelTimer->setSingleShot(true);

    connect(m_tunnelTimer, &QTimer::timeout, this, &Shprot::maybeRestartTunnelProcess);

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

    m_contextMenu = new QMenu();
    m_contextMenu->setDefaultAction(m_contextMenu->addAction(QString("%1 Preferences…").arg(PROJECT_TITLE),
                                                             this, &Shprot::openPreferencesDialog));

    m_contextMenu->addSeparator();

    QAction* showNotificationsAction = m_contextMenu->addAction("Show Notifications");
    showNotificationsAction->setCheckable(true);
    showNotificationsAction->setChecked(m_preferences->value("showNotifications", true).toBool());

    connect(showNotificationsAction, &QAction::toggled, this, [this](bool checked) {
        m_preferences->setValue("showNotifications", checked);
    });

    m_contextMenu->addSeparator();

    m_contextMenu->addAction(QString("About %1…").arg(PROJECT_TITLE), this, &Shprot::openAboutDialog);
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

    m_preferencesDialog = new PreferencesDialog(m_preferences);

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
}

void Shprot::start()
{
    m_tunnelTimer->start(0);
}

void Shprot::stop()
{
    stopTunnelProcess();
}

void Shprot::openPreferencesDialog()
{
    m_preferencesDialog->open();
}

void Shprot::openAboutDialog()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(QString("%1 version %2").arg(PROJECT_TITLE).arg(PROJECT_VERSION));
    messageBox.setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::TextSelectableByMouse);
    messageBox.setText(QString("%1<br><br><a href='%2'>GitHub repository</a>,&nbsp;"
                               "<a href='%3'>LinkedIn profile</a><br><br>%4")
                       .arg(PROJECT_DESCRIPTION)
                       .arg(PROJECT_GITHUB_REPOSITORY)
                       .arg(PROJECT_LINKEDIN_PROFILE)
                       .arg(PROJECT_COPYRIGHT));
    messageBox.exec();
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
        setLastError(QString());
        return; // SSH Proxy Tunnel is disabled
    }

    if (networkInformation->reachability() != QNetworkInformation::Reachability::Online)
    {
        return; // The Internet is not currently reachable
    }

    QString missingSshMessage = Utilities::getMissingSshToolMessage("ssh");

    if (!missingSshMessage.isEmpty())
    {
        qWarning() << qPrintable(missingSshMessage);
        setLastError(missingSshMessage);
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

    // Permissions are applied on creation only, so make sure the file does not exist yet
    QFile::remove(m_identityFilePath);
    QFile identityFile(m_identityFilePath);

    if (!identityFile.open(QIODevice::WriteOnly, QFile::ReadOwner | QFile::WriteOwner))
    {
        qCritical() << "Unable to write identity file";
        return;
    }

    QString privateKey = sshProxyTunnel.value("sshPrivateKey").toString();
    identityFile.write(privateKey.toUtf8());
    identityFile.close();

    m_tunnelSocksProxy.setType(QNetworkProxy::Socks5Proxy);
    m_tunnelSocksProxy.setHostName(localSocks5ProxyHost);
    m_tunnelSocksProxy.setPort(localSocks5ProxyPort);
    m_tunnelSocksProxy.setCapabilities(m_tunnelSocksProxy.capabilities() | QNetworkProxy::HostNameLookupCapability);

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
    arguments << "-o" << "ServerAliveInterval=15";
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
        setLastError(QString("Unable to start ssh: %1").arg(m_tunnelProcess->errorString()));
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
    m_tunnelProcess->setProperty("startedAt", QDateTime::currentMSecsSinceEpoch());

    m_healthCheckTimer->start(HealthCheckStartDelay);
    m_healthCheckFailsCount = 0;

    maybeStartHttpProxyServer();

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
        setLastError(errorDescription);
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
    maybeRestartTunnelProcessLater();
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
    healthCheckSocket->setProxy(m_tunnelSocksProxy);

    QTimer::singleShot(HealthCheckConnectionTimeout, healthCheckSocket, [healthCheckSocket]() {
        QAbstractSocket::SocketState state = healthCheckSocket->state();
        if ((state == QAbstractSocket::HostLookupState) || (state == QAbstractSocket::ConnectingState))
        {
            qWarning() << "Aborting health checker after timeout...";
            healthCheckSocket->abort();
        }
    });

    connect(healthCheckSocket, &QTcpSocket::stateChanged, this, &Shprot::handleHealthCheckSocketStateChange);

    m_healthCheckSocket = healthCheckSocket;

    quint16 healthCheckUrlPort = healthCheckUrl.port(healthCheckUrl.scheme().toLower() == "https" ? 443 : 80);
    m_healthCheckSocket->connectToHost(healthCheckUrl.host(), healthCheckUrlPort);
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

        qint64 tunnelProcessUptime = QDateTime::currentMSecsSinceEpoch() -
                                     m_tunnelProcess->property("startedAt").toLongLong();

        if (socket->property("success").toBool())
        {
            m_internetReachable = true;
            m_statusTimer->start(StatusUpdateTimeout);

            m_healthCheckFailsCount = 0;
            m_failedRestartsCount = 0;
            m_healthCheckTimer->start(HealthCheckDelayLong);

            m_lastError.clear();
            m_notifiedError.clear();
        }
        else if (tunnelProcessUptime < TunnelProcessWarmupTime)
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

            qWarning() << qPrintable(QString("Health check failed (%1) for %2")
                                     .arg(m_healthCheckFailsCount)
                                     .arg(socket->property("url").toUrl().toString()));

            if (m_healthCheckFailsCount < HealthCheckFailsLimit)
            {
                m_healthCheckTimer->start(HealthCheckDelayShort);
            }
            else
            {
                setLastError("Connected to the server, but websites are not reachable through the tunnel");
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

    if (!m_httpProxyServer->start(localHttpProxyHost, localHttpProxyPort, m_tunnelSocksProxy))
    {
        httpProxyError = QString("Local HTTP proxy is not available: %1").arg(m_httpProxyServer->errorString());
    }

    // Notify once per new problem, the tunnel itself keeps working
    if (!httpProxyError.isEmpty() && (httpProxyError != m_httpProxyError) &&
        m_preferences->value("showNotifications", true).toBool())
    {
        m_systemTrayIcon->showMessage("HTTP proxy failed", httpProxyError, QSystemTrayIcon::NoIcon);
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
        m_notifiedError.clear();
        m_httpProxyError.clear();
        m_tunnelTimer->start(0);
    }
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

    QString status;

    if (!tunnelIsEnabled)
    {
        status = "Tunnel is disabled";
    }
    else if (tunnelIsOpened)
    {
        status = "Tunnel is opened";
    }
    else
    {
        status = m_lastError.isEmpty() ? QString("Tunnel is closed") : QString("Tunnel is closed: %1").arg(m_lastError);
    }

    if (!m_httpProxyError.isEmpty())
    {
        status += "\n" + m_httpProxyError;
    }

    m_systemTrayIcon->setToolTip(QString("%1\n%2").arg(PROJECT_TITLE, status));

    if (m_tunnelIndicatedAsOpened != tunnelIsOpened)
    {
        m_tunnelIndicatedAsOpened = tunnelIsOpened;

        if (tunnelIsOpened)
        {
            m_systemTrayIcon->setIcon(m_tunnelOpenIcon);

            if (showNotifications)
            {
                m_systemTrayIcon->showMessage("Tunnel opened", "You can use your proxy", QSystemTrayIcon::NoIcon);
            }
        }
        else
        {
            m_systemTrayIcon->setIcon(m_tunnelClosedIcon);
            m_notifiedError = m_lastError;

            if (showNotifications)
            {
                QString message = m_lastError.isEmpty() ? QString("Proxy is not available") : m_lastError;
                m_systemTrayIcon->showMessage("Tunnel closed", message, QSystemTrayIcon::NoIcon);
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
            m_systemTrayIcon->showMessage("Unable to open tunnel", m_lastError, QSystemTrayIcon::NoIcon);
        }
    }
}

void Shprot::setLastError(const QString& error)
{
    m_lastError = error;
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
            QLocalSocket singleInstanceSocket;
            singleInstanceSocket.connectToServer(getSingleInstanceServerName(userName));

            if (singleInstanceSocket.waitForConnected(SingleInstanceWaitTimeout))
            {
                singleInstanceSocket.write("quit");
                singleInstanceSocket.waitForBytesWritten(SingleInstanceWriteTimeout);
                singleInstanceSocket.close();
            }
        }

        return EXIT_SUCCESS;
    }

    if (arguments.contains("--delay"))
    {
        QThread::msleep(StartDelayTimeout);
    }

    QApplication application(argc, argv);

    // Check for other instances already running

    QString singleInstanceServerName = getSingleInstanceServerName(Utilities::getCurrentUserName());

    QLocalSocket singleInstanceSocket;
    singleInstanceSocket.connectToServer(singleInstanceServerName);

    if (singleInstanceSocket.waitForConnected(SingleInstanceWaitTimeout))
    {
        // Application is already started
        singleInstanceSocket.write("activate");
        singleInstanceSocket.waitForBytesWritten(SingleInstanceWriteTimeout);
        singleInstanceSocket.close();
        return EXIT_FAILURE;
    }

    // Initialize single instance server to prevent other instances to start

    QLocalServer::removeServer(singleInstanceServerName);
    QLocalServer singleInstanceServer;

#if defined(Q_OS_LINUX)
    // Allow the owner only: root bypasses file permissions, so package scripts can still send "quit".
    // Windows named pipes are already writable by the owner, administrators and LocalSystem only.
    singleInstanceServer.setSocketOptions(QLocalServer::UserAccessOption);
#endif

    if (!singleInstanceServer.listen(singleInstanceServerName))
    {
        return EXIT_FAILURE; // Unable to start single instance server
    }

    if (!QNetworkInformation::loadDefaultBackend())
    {
        qWarning() << "Unable to load default network backend.";
    }

    QApplication::setWindowIcon(QIcon(":/Shprot.png"));
    QApplication::setStyle(new TweakStyle(application.style()));
    QApplication::setQuitOnLastWindowClosed(false);

    Shprot shprot;
    shprot.start();

    QObject::connect(&singleInstanceServer, &QLocalServer::newConnection, [&singleInstanceServer, &shprot]() {
        while (singleInstanceServer.hasPendingConnections())
        {
            QLocalSocket* socket = singleInstanceServer.nextPendingConnection();

            if (!socket)
            {
                return;
            }

            // The message is complete when the sender disconnects
            std::shared_ptr<QByteArray> message = std::make_shared<QByteArray>();

            auto readMessage = [socket, message]() {
                if (socket->bytesAvailable() <= 0)
                {
                    return; // Nothing to read (reading a closed socket produces warnings)
                }

                message->append(socket->readAll());

                if (message->size() > SingleInstanceMessageSizeMax)
                {
                    message->clear();
                    socket->abort(); // Not a message from another instance
                }
            };

            auto handleMessage = [socket, message, &shprot]() {
                if (socket->property("handled").toBool())
                {
                    return;
                }

                socket->setProperty("handled", true);
                socket->deleteLater();

                if (*message == "activate")
                {
                    shprot.openPreferencesDialog();
                }
                else if (*message == "quit")
                {
                    qApp->quit();
                }
            };

            QObject::connect(socket, &QLocalSocket::readyRead, socket, readMessage);
            QObject::connect(socket, &QLocalSocket::disconnected, socket, [readMessage, handleMessage]() {
                readMessage();
                handleMessage();
            });

            QTimer::singleShot(SingleInstanceReadTimeout, socket, [socket]() {
                socket->abort(); // Sender did not disconnect in time
            });

            readMessage(); // Sender may have already sent the message and disconnected

            if (socket->state() == QLocalSocket::UnconnectedState)
            {
                handleMessage();
            }
        }
    });

    if (arguments.contains("--preferences"))
    {
        shprot.openPreferencesDialog();
    }

    return application.exec();
}
