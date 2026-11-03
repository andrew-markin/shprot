#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QTimer>
#include <QUrl>

class Preferences;

// Periodically asks GitHub for the latest release. TLS is provided by the system (Schannel on Windows,
// OpenSSL loaded at runtime on Linux), so no TLS libraries are shipped with the application.
// The latest version found is kept in preferences, so a new version is offered right after restart.
class UpdateChecker : public QObject
{
    Q_OBJECT

public:
    UpdateChecker(Preferences* preferences, QObject* parent = nullptr);

    void start();
    void setProxy(const QNetworkProxy& proxy);

    QString newVersion() const; // Empty unless the latest release is newer than this application
    QUrl newVersionUrl() const;

    // Release tags look like "v1.6", anything else is not a version
    static QString parseTagVersion(const QString& tag);
    static bool isNewerVersion(const QString& version, const QString& currentVersion);

signals:
    void latestVersionChanged();

private slots:
    void check();
    void handleReplyFinish();

private:
    Preferences* m_preferences;
    QNetworkAccessManager* m_networkAccessManager;
    QNetworkProxy m_proxy = QNetworkProxy(QNetworkProxy::NoProxy);
    QNetworkReply* m_reply = nullptr;
    QTimer* m_checkTimer;

    QString m_latestVersion;
};

#endif // UPDATECHECKER_H
