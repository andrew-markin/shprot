#include "Utilities.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QHostAddress>
#include <QHostInfo>
#include <QProcess>
#include <QProcessEnvironment>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QSaveFile>
#include <QScopeGuard>
#include <QStandardPaths>
#include <QTextStream>

namespace Utilities {

QString getRandom128BitHexKey()
{
    QByteArray result(16, 0);
    QRandomGenerator::global()->fillRange(reinterpret_cast<quint32*>(result.data()), 4);
    return result.toHex();
}

QString getTemporaryFilePath()
{
    QString temporaryDirPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    return QDir(temporaryDirPath).filePath(QString("%1-%2").arg(PROJECT_NAME).arg(getRandom128BitHexKey()));
}

QString getCurrentUserName()
{
#ifdef Q_OS_WIN
    return QProcessEnvironment::systemEnvironment().value("USERNAME");
#else
    return QProcessEnvironment::systemEnvironment().value("USER");
#endif
}

// Returns an empty string if the OpenSSH tool is available, or a hint on how to install it otherwise
QString getMissingSshToolMessage(const QString& tool)
{
    if (!QStandardPaths::findExecutable(tool).isEmpty())
    {
        return QString();
    }

#ifdef Q_OS_WIN
    return QString("OpenSSH client (%1) is not installed. Install \"OpenSSH Client\" "
                   "in Windows Settings, Optional features.").arg(tool);
#else
    return QString("OpenSSH client (%1) is not installed. "
                   "Install the OpenSSH client package (e.g. openssh-client).").arg(tool);
#endif
}

QString generateSshPrivateKey()
{
    QString result;

    QString privateKeyPath = getTemporaryFilePath();
    QString publicKeyPath = privateKeyPath + ".pub";

    auto cleanup = qScopeGuard([privateKeyPath, publicKeyPath] {
        QFile::remove(privateKeyPath);
        QFile::remove(publicKeyPath);
    });

    QString userName = getCurrentUserName();
    QString hostName = QHostInfo::localHostName();
    QString privateKeyComment = QString("%1@%2/%3").arg(userName).arg(hostName).arg(PROJECT_NAME);

    QProcess process;
    process.start("ssh-keygen", {"-t", "ed25519", "-f", privateKeyPath, "-N", "", "-C", privateKeyComment, "-q"});

    if (!process.waitForFinished(5000))
    {
        qWarning() << "ssh-keygen process timed out or failed to start";
        return QString();
    }

    if (process.exitCode() != 0)
    {
        qWarning() << "ssh-keygen error output:" << process.readAllStandardError();
        return QString();
    }

    QFile privateKeyFile(privateKeyPath);

    if (privateKeyFile.open(QIODevice::ReadOnly))
    {
        result = QString::fromUtf8(privateKeyFile.readAll());
        privateKeyFile.close();
    }

    return result;
}

QString getSshPublicKey(const QString& privateKey)
{
    QString privateKeyPath = getTemporaryFilePath();

    auto cleanup = qScopeGuard([privateKeyPath] {
        QFile::remove(privateKeyPath);
    });

    QFile privateKeyFile(privateKeyPath);

    if (!privateKeyFile.open(QIODevice::WriteOnly | QIODevice::NewOnly, QFile::ReadOwner | QFile::WriteOwner))
    {
        qWarning() << "Unable to create temporary file";
        return QString();
    }

    privateKeyFile.write(privateKey.toUtf8());
    privateKeyFile.close();

    QProcess process;
    process.start("ssh-keygen", {"-y", "-f", privateKeyPath});

    if (!process.waitForStarted(2000))
    {
        qWarning() << "Failed to start ssh-keygen:" << process.errorString();
        return QString();
    }

    process.write(privateKey.toUtf8());
    process.closeWriteChannel();

    if (!process.waitForFinished(3000))
    {
        qWarning() << "ssh-keygen process timed out";
        process.kill();
        return QString();
    }

    if (process.exitCode() != 0) {
        qWarning() << "ssh-keygen failed with error:" << process.readAllStandardError();
        return QString();
    }

    return QString::fromUtf8(process.readAllStandardOutput().trimmed());
}

bool sshPrivateKeyLooksValid(const QString& privateKey)
{
    if (privateKey.isEmpty() || privateKey.size() > 32768)
    {
        return false;
    }

    static const QRegularExpression SshPrivateKeyRegEx(
                R"(^-----BEGIN [A-Z0-9 ]+ PRIVATE KEY-----\n.+\n-----END [A-Z0-9 ]+ PRIVATE KEY-----$)",
                QRegularExpression::DotMatchesEverythingOption);

    return SshPrivateKeyRegEx.match(privateKey).hasMatch();
}

QString getAddressFromSshDestination(const QString& destination)
{
    static QRegularExpression AddressRegEx(R"(^(?:[a-zA-Z0-9._-]+@)?(?<address>[a-zA-Z0-9.-]+(?::\d+)?)$)");
    return AddressRegEx.match(destination).captured("address");
}

QString getKnownHostsPath(const QString& address)
{
    QString addressHash = QCryptographicHash::hash(address.toLower().toUtf8(), QCryptographicHash::Sha256).toHex();
    QDir appConfigDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    QDir knownHostsRootDir(appConfigDir.filePath("KnownHosts"));
    return knownHostsRootDir.filePath(addressHash);
}

QString readFirstHostRecord(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return QString();
    }

    QTextStream fileStream(&file);
    fileStream.setEncoding(QStringConverter::Utf8);

    while (!fileStream.atEnd())
    {
        QString line = fileStream.readLine().trimmed();

        if (line.isEmpty() || line.startsWith('#') || line.startsWith('@'))
        {
            continue; // Skip empty lines, comments and markers (e.g. @cert-authority or @revoked)
        }

        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        if (parts.size() >= 3)
        {
            return parts.first(3).join(' ');
        }
    }

    return QString();
}

QString probeActualHostRecord(const QString& address, quint16 port)
{
    QString knownHostsPath = getTemporaryFilePath();

    auto cleanup = qScopeGuard([knownHostsPath] {
        QFile::remove(knownHostsPath);
    });

    QString probeUser = QString("probe-%1").arg(getRandom128BitHexKey().first(20));

    QProcess process;
    QStringList arguments;
    arguments << QString("%1@%2").arg(probeUser).arg(address) << "-p" << QString::number(port) << "-N";
    arguments << "-o" << QString("UserKnownHostsFile='%1'").arg(QDir::toNativeSeparators(knownHostsPath));
    arguments << "-o" << "StrictHostKeyChecking=accept-new";
    arguments << "-o" << "BatchMode=yes";

    process.start("ssh", arguments);

    if (!process.waitForFinished(5000))
    {
        process.kill();
        qCritical() << "Server probe timeout";
        return QString();
    }

    return readFirstHostRecord(knownHostsPath);
}

QString getKnownHostRecord(const QString& address)
{
    QString knownHostsPath = getKnownHostsPath(address);
    return readFirstHostRecord(knownHostsPath);
}

bool setKnownHostRecord(const QString& address, const QString& record)
{
    QString knownHostsPath = getKnownHostsPath(address);

    // Make sure known hosts root directory exists
    QFileInfo knownHostsFileInfo(knownHostsPath);
    knownHostsFileInfo.dir().mkpath(".");

    QSaveFile knownHostsFile(knownHostsPath);

    if (!knownHostsFile.open(QIODevice::WriteOnly))
    {
        qCritical() << "Unable to open known hosts file for writing:" << knownHostsPath;
        return false;
    }

    QByteArray data = record.toUtf8();
    qint64 bytesWritten = knownHostsFile.write(data);

    if (bytesWritten == -1)
    {
        qWarning() << "Host record saving error:" << knownHostsFile.errorString();
        return false;
    }

    if (bytesWritten != data.size())
    {
        qWarning() << "Host record was not saved completely: Only" << bytesWritten << "of" << data.size();
        return false;
    }

    if (!knownHostsFile.commit())
    {
        qWarning() << "Unable to save host record file:" << knownHostsFile.errorString();
        return false;
    }

    return true;
}

QString getKeyTypeLabel(const QString& keyType)
{
    static const QMap<QString, QString> KeyTypeLabels = {
        {"ssh-ed25519", "ED25519"},
        {"ecdsa-sha2-nistp256", "ECDSA"},
        {"ecdsa-sha2-nistp384", "ECDSA"},
        {"ecdsa-sha2-nistp521", "ECDSA"},
        {"ssh-rsa", "RSA"},
        {"ssh-dss", "DSA"},
        {"sk-ssh-ed25519@openssh.com", "ED25519-SK"},
        {"sk-ecdsa-sha2-nistp256@openssh.com", "ECDSA-SK"},
        {"ssh-ed25519-cert-v01@openssh.com", "ED25519-CERT"},
        {"ssh-rsa-cert-v01@openssh.com", "RSA-CERT"}
    };

    return KeyTypeLabels.value(keyType.toLower(), keyType.toUpper());
}

QPair<QString, QString> getKeyTypeAndFingerprint(const QString& hostRecord)
{
    QStringList parts = hostRecord.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QString keyType = parts.value(1);
    QByteArray keyData = QByteArray::fromBase64(parts.value(2).toUtf8());
    QByteArray keyFingerprint = QCryptographicHash::hash(keyData, QCryptographicHash::Sha256)
                                .toBase64(QByteArray::OmitTrailingEquals);
    return { keyType, keyFingerprint };
}

quint16 parsePort(const QString& text, quint16 defaultValue)
{
    bool resultIsOk = false;
    quint16 result = text.toUShort(&resultIsOk);
    return resultIsOk ? result : defaultValue;
}

// Parses "host", "host:port" or "[ipv6]:port" (user info is ignored)
bool parseAuthority(const QByteArray& authority, quint16 defaultPort, QString& host, quint16& port)
{
    QByteArray hostAndPort = authority.mid(authority.lastIndexOf('@') + 1);
    QByteArray portPart;

    if (hostAndPort.startsWith('['))
    {
        int end = hostAndPort.indexOf(']');

        if (end < 0)
        {
            return false;
        }

        host = QString::fromLatin1(hostAndPort.mid(1, end - 1));
        portPart = hostAndPort.mid(end + 1);

        if (QHostAddress(host).protocol() != QAbstractSocket::IPv6Protocol)
        {
            return false;
        }
    }
    else
    {
        int colon = hostAndPort.indexOf(':');

        if ((colon >= 0) && (hostAndPort.lastIndexOf(':') != colon))
        {
            return false; // IPv6 address must be enclosed in brackets
        }

        host = QString::fromLatin1(colon >= 0 ? hostAndPort.left(colon) : hostAndPort);
        portPart = (colon >= 0) ? hostAndPort.mid(colon) : QByteArray();
    }

    if (host.isEmpty())
    {
        return false;
    }

    if (portPart.isEmpty())
    {
        port = defaultPort;
        return true;
    }

    if (!portPart.startsWith(':'))
    {
        return false;
    }

    bool portIsOk = false;
    port = portPart.mid(1).toUShort(&portIsOk);
    return portIsOk && (port > 0);
}

// Anything except loopback is reachable from other computers. Host names other than "localhost"
// are treated as exposed too, since they are not resolved here and may point anywhere.
bool hostIsExposed(const QString& host)
{
    QString normalizedHost = host.trimmed();

    if (normalizedHost.isEmpty() || (normalizedHost.compare("localhost", Qt::CaseInsensitive) == 0))
    {
        return false;
    }

    if (normalizedHost.startsWith('[') && normalizedHost.endsWith(']'))
    {
        normalizedHost = normalizedHost.mid(1, normalizedHost.size() - 2);
    }

    QHostAddress address(normalizedHost);
    return address.isNull() || !address.isLoopback();
}

// Turns ssh error output into a short explanation for the user
QString describeSshError(const QByteArray& errorOutput)
{
    static const QList<QPair<QStringList, QString>> KnownErrors = {
        {{"Permission denied (", "Too many authentication failures"},
         "Authentication failed: the server rejected the private key"},
        {{"Host key verification failed", "REMOTE HOST IDENTIFICATION HAS CHANGED"},
         "Server identity verification failed, run Trust Check in Preferences"},
        {{"Could not resolve hostname"}, "Unable to resolve the server address"},
        {{"Connection refused"}, "Connection refused by the server"},
        {{"Connection timed out", "Operation timed out"}, "Connection to the server timed out"},
        {{"Network is unreachable", "No route to host"}, "Server is unreachable"},
        {{"Address already in use", "cannot listen to port"}, "Local SOCKS5 proxy port is already in use"},
        {{"Timeout, server"}, "Server stopped responding"},
        {{"closed by remote host", "Connection reset", "Broken pipe"}, "Connection was closed by the server"}
    };

    QString output = QString::fromUtf8(errorOutput);

    for (const auto& [patterns, description] : KnownErrors)
    {
        for (const QString& pattern : patterns)
        {
            if (output.contains(pattern, Qt::CaseInsensitive))
            {
                return description;
            }
        }
    }

    // Unknown error: the last line of ssh output usually explains it best
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    return lines.isEmpty() ? QString() : lines.last().trimmed().left(200);
}

} // namespace Utilities
