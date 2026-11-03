#include "UpdateChecker.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInformation>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QVersionNumber>

#include "Preferences.h"

namespace {

// The application usually starts at login, when the network and the tunnel may not be ready yet
static const int CheckDelayFirst = 60000; // 1m
static const int CheckDelayRegular = 43200000; // 12h
static const int CheckDelayFailed = 3600000; // 1h

static const int CheckTransferTimeout = 30000; // 30s

// Only small JSON is expected, a bigger reply is not from GitHub API
static const qint64 ReplySizeMax = 1024 * 1024;

} // namespace

UpdateChecker::UpdateChecker(Preferences* preferences, QObject* parent) : QObject(parent), m_preferences(preferences)
{
    // Stored as a version, the tag is restored from it
    m_latestVersion = parseTagVersion("v" + m_preferences->value("latestVersion").toString());

    m_networkAccessManager = new QNetworkAccessManager(this);

    m_checkTimer = new QTimer(this);
    m_checkTimer->setSingleShot(true);

    connect(m_checkTimer, &QTimer::timeout, this, &UpdateChecker::check);
}

void UpdateChecker::start()
{
    m_checkTimer->start(CheckDelayFirst);
}

void UpdateChecker::setProxy(const QNetworkProxy& proxy)
{
    m_proxy = proxy; // Used starting from the next check
}

QString UpdateChecker::newVersion() const
{
    return isNewerVersion(m_latestVersion, PROJECT_VERSION) ? m_latestVersion : QString();
}

QUrl UpdateChecker::newVersionUrl() const
{
    // Release page address is built locally instead of being taken from the reply
    return QUrl(QString("%1/releases/tag/v%2").arg(PROJECT_GITHUB_REPOSITORY, m_latestVersion));
}

QString UpdateChecker::parseTagVersion(const QString& tag)
{
    static const QRegularExpression tagRegExp("^v(\\d{1,5}(\\.\\d{1,5}){1,3})$");
    return tagRegExp.match(tag).captured(1);
}

bool UpdateChecker::isNewerVersion(const QString& version, const QString& currentVersion)
{
    return QVersionNumber::fromString(version) > QVersionNumber::fromString(currentVersion);
}

void UpdateChecker::check()
{
    if (m_reply)
    {
        return; // Previous check is still in progress
    }

    QNetworkInformation* networkInformation = QNetworkInformation::instance();

    if (networkInformation && (networkInformation->reachability() != QNetworkInformation::Reachability::Online))
    {
        m_checkTimer->start(CheckDelayFailed);
        return; // The Internet is not currently reachable
    }

    // Repository path is taken from its URL, e.g. "/andrew-markin/shprot"
    QUrl url(QString("https://api.github.com/repos%1/releases/latest").arg(QUrl(PROJECT_GITHUB_REPOSITORY).path()));

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, QString("%1/%2").arg(PROJECT_NAME, PROJECT_VERSION));
    request.setRawHeader("Accept", "application/vnd.github+json");
    request.setTransferTimeout(CheckTransferTimeout);

    m_networkAccessManager->setProxy(m_proxy);
    m_reply = m_networkAccessManager->get(request);

    connect(m_reply, &QNetworkReply::finished, this, &UpdateChecker::handleReplyFinish);
}

void UpdateChecker::handleReplyFinish()
{
    QNetworkReply* reply = m_reply;
    m_reply = nullptr;
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "Unable to check for updates:" << reply->errorString();
        m_checkTimer->start(CheckDelayFailed);
        return;
    }

    QByteArray data = reply->read(ReplySizeMax);
    QJsonObject release = QJsonDocument::fromJson(data).object();
    QString tag = release.value("tag_name").toString();
    QString version = parseTagVersion(tag);

    if (version.isEmpty())
    {
        qWarning() << "Unable to check for updates: unexpected reply";
        m_checkTimer->start(CheckDelayFailed);
        return;
    }

    m_checkTimer->start(CheckDelayRegular);

    if (version == m_latestVersion)
    {
        return; // Preferences are not rewritten on every check
    }

    // Saved even when it's not newer, so a withdrawn release stops being offered
    m_latestVersion = version;
    m_preferences->setValue("latestVersion", version);

    if (isNewerVersion(version, PROJECT_VERSION))
    {
        qInfo() << qPrintable(QString("New version is available: %1").arg(version));
    }

    emit latestVersionChanged();
}
