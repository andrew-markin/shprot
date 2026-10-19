#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>

namespace Utilities {

QString getCurrentUserName();

QString getMissingSshToolMessage(const QString& tool);

bool writePrivateKeyFile(const QString& path, const QByteArray& privateKey);

QString generateSshPrivateKey();
QString getSshPublicKey(const QString& privateKey);
bool sshPrivateKeyLooksValid(const QString& privateKey);

QString getAddressFromSshDestination(const QString& destination);

QString getKnownHostsPath(const QString& address);

QString probeActualHostRecord(const QString& address, quint16 port = 22);
QString getKnownHostRecord(const QString& address);
bool setKnownHostRecord(const QString& address, const QString& record);

QString getKeyTypeLabel(const QString& keyType);
QPair<QString, QString> getKeyTypeAndFingerprint(const QString& hostRecord);

quint16 parsePort(const QString& text, quint16 defaultValue);
bool parseAuthority(const QByteArray& authority, quint16 defaultPort, QString& host, quint16& port);
bool hostIsExposed(const QString& host);

QString describeSshError(const QByteArray& errorOutput);

} // namespace Utilities

#endif // UTILITIES_H
