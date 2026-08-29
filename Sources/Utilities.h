#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>

namespace Utilities {

QString getCurrentUserName();

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

} // namespace Utilities

#endif // UTILITIES_H
