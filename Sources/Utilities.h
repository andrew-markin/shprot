#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>

namespace Utilities {

QString generateSshPrivateKey();
QString getSshPublicKey(const QString& privateKey);
bool sshPrivateKeyLooksValid(const QString& privateKey);

QString getAddressFromSshDestination(const QString& destination);

QString getKnownHostsPath(const QString& address);

QString probeActualHostRecord(const QString& address);
QString getKnownHostRecord(const QString& address);
bool setKnownHostRecord(const QString& address, const QString& record);

QString getKeyTypeLabel(const QString& keyType);
QPair<QString, QString> getKeyTypeAndFingerprint(const QString& hostRecord);

} // namespace Utilities

#endif // UTILITIES_H
