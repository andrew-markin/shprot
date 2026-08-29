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

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#include <wtsapi32.h>
#elif defined(Q_OS_LINUX)
#include <utmp.h>
#endif

#include "HttpProxyServer.h"
#include "Preferences.h"
#include "PreferencesDialog.h"
#include "TweakStyle.h"
#include "Utilities.h"
#include "Websites.h"

namespace {

static const int StartDelayTimeout = 5000; // 5s

static const int SingleInstanceWaitTimeout = 500; // 500ms
static const int SingleInstanceReadTimeout = 5000; // 5s
static const int SingleInstanceWriteTimeout = 5000; // 5s

static const int TunnelProcessStartTimeout = 2000; // 2s
static const int TunnelProcessUptimeLimit = 1000; // 1s
static const int TunnelProcessRestartDelayLazy = 1000; // 1s
static const int TunnelProcessRestartDelayStep = 200; // 200ms
static const int TunnelProcessRestartDelayMax = 10000; // 10s

static const int HealthCheckStartDelay = 500; // 500ms
static const int HealthCheckDelayLong = 20000; // 20s
static const int HealthCheckDelayShort = 50; // 50ms
static const int HealthCheckConnectionTimeout = 3000; // 3s
static const int HealthCheckFailsLimit = 3;

static const int StatusUpdateTimeout = 1000; // 1s

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
    setutent();
    struct utmp* entry;
    QSet<QString> userNamesSet;

    while ((entry = getutent()) != nullptr)
    {
        if (entry->ut_type == USER_PROCESS)
        {
            QString username = QString::fromLocal8Bit(entry->ut_user);

            if (!username.isEmpty())
            {
                userNamesSet.insert(username);
            }
        }
    }

    endutent();
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
    m_contextMenu->setDefaultAction(m_contextMenu->addAction(tr("Preferences…"), this, &Shprot::openPreferencesDialog));
    m_contextMenu->addAction(tr("About %1").arg(PROJECT_TITLE), this, &Shprot::openAboutDialog);

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

    m_preferencesDialog = new PreferencesDialog(m_preferences);

    QNetworkInformation* networkInformation = QNetworkInformation::instance();

    connect(networkInformation, &QNetworkInformation::reachabilityChanged,
            this, &Shprot::maybeRestartTunnelProcessLater);

    connect(networkInformation, &QNetworkInformation::transportMediumChanged,
            this, &Shprot::maybeRestartTunnelProcessLater);

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

    if (!sshProxyTunnel.value("enabled", false).toBool())
    {
        return; // SSH Proxy Tunnel is disabled
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

    QFile identityFile(m_identityFilePath);

    if (!identityFile.open(QIODevice::WriteOnly))
    {
        qCritical() << "Unable to write identity file";
        return;
    }

    QString privateKey = sshProxyTunnel.value("sshPrivateKey").toString();
    identityFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
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
    arguments << "-o" << QString("IdentityFile=%1").arg(QDir::toNativeSeparators(m_identityFilePath));
    arguments << "-o" << QString("UserKnownHostsFile=%1").arg(QDir::toNativeSeparators(knownHostsPath));
    arguments << "-o" << "StrictHostKeyChecking=yes";
    arguments << "-o" << "BatchMode=yes";

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
        m_frequentRestartsCount = 0;
        return;
    }

    qint64 uptime = QDateTime::currentMSecsSinceEpoch() - m_tunnelProcess->property("startedAt").toLongLong();

    if (uptime < TunnelProcessUptimeLimit)
    {
        m_frequentRestartsCount++;
    }
    else
    {
        m_frequentRestartsCount = 0;
    }

    m_tunnelTimer->start(qMin(m_frequentRestartsCount * TunnelProcessRestartDelayStep, TunnelProcessRestartDelayMax));
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
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

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

        if (socket->property("success").toBool())
        {
            m_healthCheckFailsCount = 0;
            m_healthCheckTimer->start(HealthCheckDelayLong);

            m_internetReachable = true;
            m_statusTimer->start(StatusUpdateTimeout);
        }
        else
        {
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
                maybeRestartTunnelProcessLater();
            }

            m_internetReachable = false;
            m_statusTimer->start(StatusUpdateTimeout);
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
        return; // SSH Proxy Tunnel or Local HTTP Proxy are disabled
    }

    QString localHttpProxyHost = sshProxyTunnel.value("localHttpProxyHost", "localhost").toString();

    if (localHttpProxyHost.isEmpty())
    {
        localHttpProxyHost = "localhost";
    }

    quint16 localHttpProxyPort = Utilities::parsePort(sshProxyTunnel.value("localHttpProxyPort").toString(), 8080);
    m_httpProxyServer->start(localHttpProxyHost, localHttpProxyPort, m_tunnelSocksProxy);
}

void Shprot::handlePreferencesChange(const QStringList& changes)
{
    if (changes.contains("sshProxyTunnel"))
    {
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
    bool tunnelIsOpened = (m_tunnelProcess->state() == QProcess::Running) && m_internetReachable;

    if (m_tunnelIndicatedAsOpened == tunnelIsOpened)
    {
        return;
    }

    if (tunnelIsOpened)
    {
        m_systemTrayIcon->setIcon(m_tunnelOpenIcon);
        m_systemTrayIcon->showMessage("Tunnel opened", "You can use your proxy", QSystemTrayIcon::NoIcon);
    }
    else
    {
        m_systemTrayIcon->setIcon(m_tunnelClosedIcon);
        m_systemTrayIcon->showMessage("Tunnel closed", "Proxy is not available", QSystemTrayIcon::NoIcon);
    }

    m_tunnelIndicatedAsOpened = tunnelIsOpened;
}

// Main function

int main(int argc, char* argv[])
{
    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] %{message}");

    QApplication::setApplicationName(PROJECT_NAME);
    QApplication::setQuitOnLastWindowClosed(false);

    QApplication application(argc, argv);
    QStringList arguments = QCoreApplication::arguments();

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

    if (!singleInstanceServer.listen(singleInstanceServerName))
    {
        return EXIT_FAILURE; // Unable to start single instance server
    }

    if (!QNetworkInformation::loadDefaultBackend())
    {
        qWarning() << "Unable to load default network backend.";
    }

    application.setWindowIcon(QIcon(":/Shprot.png"));
    application.setStyle(new TweakStyle(application.style()));

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

            QByteArray message;
            QElapsedTimer timer;
            timer.start();

            while (((socket->state() == QLocalSocket::ConnectedState) || (socket->bytesAvailable() > 0)) &&
                   (timer.elapsed() < SingleInstanceReadTimeout))
            {
                if (socket->bytesAvailable() <= 0)
                {
                    socket->waitForReadyRead(SingleInstanceReadTimeout);
                }

                message.append(socket->readAll());
            }

            socket->deleteLater();

            if (message == "activate")
            {
                shprot.openPreferencesDialog();
            }
            else if (message == "quit")
            {
                qApp->quit();
            }
        }
    });

    if (arguments.contains("--preferences"))
    {
        shprot.openPreferencesDialog();
    }

    return application.exec();
}
