#ifndef SHPROT_H
#define SHPROT_H

#include <QElapsedTimer>
#include <QMenu>
#include <QNetworkInformation>
#include <QNetworkProxy>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QTcpSocket>
#include <QTimer>

class AboutDialog;
class HttpProxyServer;
class LogsDialog;
class Preferences;
class PreferencesDialog;
class UpdateChecker;

class Shprot : public QObject
{
    Q_OBJECT

public:
    Shprot(Preferences* preferences, QObject* parent = nullptr);
    ~Shprot();

public slots:
    void start();
    void stop();

    void openPreferencesDialog();
    void openLogsDialog();
    void openAboutDialog();

private slots:
    void maybeRestartTunnelProcess();
    void maybeRestartTunnelProcessLater();
    void stopTunnelProcess(int timeout = 3000);
    void scheduleTunnelProcessRestart(bool authenticationFailed);
    void handleNetworkChange();

    void handleTunnelProcessStart();
    void handleTunnelProcessFinish(int exitCode, QProcess::ExitStatus exitStatus);

    void stopHealthCheck();
    void maybeRunHealthCheck();
    void handleHealthCheckSocketStateChange(QAbstractSocket::SocketState state);

    void maybeStartHttpProxyServer();

    void handlePreferencesChange(const QStringList& changes);
    void setTunnelEnabled(bool enabled);
    void handleSystemTrayIconActivation(QSystemTrayIcon::ActivationReason reason);

    void maybeUpdateStatus();
    void updateStatusText();
    void updateNewVersionAction();
    void setLastError(const QString& error, const QString& errorSummary);

private:
    Preferences* m_preferences;
    QString m_identityFilePath;

    QTimer* m_tunnelTimer;
    QProcess* m_tunnelProcess;
    QNetworkProxy m_tunnelSocksProxy;
    bool m_tunnelStoppedIntentionally = false;
    int m_failedRestartsCount = 0;
    bool m_tunnelPortOpened = false; // Local port is opened by ssh only after authentication
    QElapsedTimer m_tunnelWarmupTimer;

    QTimer* m_healthCheckTimer;
    QTcpSocket* m_healthCheckSocket = nullptr;

    QList<QUrl> m_healthCheckUrls;
    int m_healthCheckUrlIndex = 0;
    int m_healthCheckFailsCount = 0;

    HttpProxyServer* m_httpProxyServer;

    QMenu* m_contextMenu;
    QAction* m_statusAction;
    QAction* m_reconnectAction;
    QAction* m_tunnelEnabledAction;
    QAction* m_copySocks5ProxyUrlAction;
    QAction* m_copyHttpProxyUrlAction;
    QAction* m_newVersionAction;
    QString m_socks5ProxyUrl; // Empty when the proxy is not started
    QString m_httpProxyUrl;
    QIcon m_tunnelOpenIcon;
    QIcon m_tunnelClosedIcon;
    QSystemTrayIcon* m_systemTrayIcon;
    QElapsedTimer m_systemTrayIconTriggerTimer;

    QTimer* m_statusTimer;
    QTimer* m_statusCountdownTimer;
    bool m_internetReachable = false;
    bool m_tunnelIndicatedAsOpened = false;
    QString m_lastError; // Why the tunnel is not opened, shown in notifications and tray icon tool tip
    QString m_lastErrorSummary; // The same reason in a few words, so it doesn't stretch the menu
    QString m_notifiedError;
    QString m_httpProxyError;

    UpdateChecker* m_updateChecker;

    PreferencesDialog* m_preferencesDialog;
    LogsDialog* m_logsDialog = nullptr;
    AboutDialog* m_aboutDialog = nullptr;
};

#endif // SHPROT_H
