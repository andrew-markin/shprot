#include <QFile>
#include <QProcess>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTest>

#include "Utilities.h"

class TestUtilities : public QObject
{
    Q_OBJECT

private slots:
    void parsePort_data();
    void parsePort();

    void getAddressFromSshDestination_data();
    void getAddressFromSshDestination();

    void parseAuthority_data();
    void parseAuthority();

    void hostIsExposed_data();
    void hostIsExposed();

    void getProxyConnectionHost_data();
    void getProxyConnectionHost();

    void formatProxyUrl_data();
    void formatProxyUrl();

    void proxyAddressesConflict_data();
    void proxyAddressesConflict();

    void formatUptime_data();
    void formatUptime();

    void describeSshError_data();
    void describeSshError();

    void summarizeSshError_data();
    void summarizeSshError();

    void sshPrivateKeyLooksValid_data();
    void sshPrivateKeyLooksValid();

    void sshPrivateKeyIsEncrypted_data();
    void sshPrivateKeyIsEncrypted();

    void generatedKeysEncryption_data();
    void generatedKeysEncryption();

    void getKeyTypeLabel_data();
    void getKeyTypeLabel();

    void getKeyTypeAndFingerprint();

    void getMissingSshToolMessage();

    void writePrivateKeyFile();

    void generateSshKeys();

    void getSshKeyInstallCommand_data();
    void getSshKeyInstallCommand();
};

void TestUtilities::parsePort_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("expected");

    QTest::newRow("valid") << "2222" << 2222;
    QTest::newRow("max") << "65535" << 65535;
    QTest::newRow("empty") << "" << 22;
    QTest::newRow("not a number") << "ssh" << 22;
    QTest::newRow("too large") << "70000" << 22;
    QTest::newRow("negative") << "-1" << 22;
}

void TestUtilities::parsePort()
{
    QFETCH(QString, text);
    QFETCH(int, expected);

    QCOMPARE(Utilities::parsePort(text, 22), quint16(expected));
}

void TestUtilities::getAddressFromSshDestination_data()
{
    QTest::addColumn<QString>("destination");
    QTest::addColumn<QString>("expected");

    QTest::newRow("user and host") << "tunnel@myserver.com" << "myserver.com";
    QTest::newRow("host only") << "myserver.com" << "myserver.com";
    QTest::newRow("ip address") << "tunnel@192.0.2.1" << "192.0.2.1";
    QTest::newRow("invalid characters") << "tunnel@my server" << "";
    QTest::newRow("empty") << "" << "";
}

void TestUtilities::getAddressFromSshDestination()
{
    QFETCH(QString, destination);
    QFETCH(QString, expected);

    QCOMPARE(Utilities::getAddressFromSshDestination(destination), expected);
}

void TestUtilities::parseAuthority_data()
{
    QTest::addColumn<QByteArray>("authority");
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("host");
    QTest::addColumn<int>("port");

    QTest::newRow("host") << QByteArray("example.com") << true << "example.com" << 80;
    QTest::newRow("host and port") << QByteArray("example.com:8080") << true << "example.com" << 8080;
    QTest::newRow("user info") << QByteArray("user:secret@example.com:81") << true << "example.com" << 81;
    QTest::newRow("ipv4") << QByteArray("192.0.2.1:443") << true << "192.0.2.1" << 443;
    QTest::newRow("ipv6") << QByteArray("[::1]") << true << "::1" << 80;
    QTest::newRow("ipv6 and port") << QByteArray("[2001:db8::1]:443") << true << "2001:db8::1" << 443;

    QTest::newRow("ipv6 without brackets") << QByteArray("::1:443") << false << "" << 0;
    QTest::newRow("unclosed bracket") << QByteArray("[::1:443") << false << "" << 0;
    QTest::newRow("host in brackets") << QByteArray("[example.com]:80") << false << "" << 0;
    QTest::newRow("empty port") << QByteArray("example.com:") << false << "" << 0;
    QTest::newRow("zero port") << QByteArray("example.com:0") << false << "" << 0;
    QTest::newRow("port too large") << QByteArray("example.com:70000") << false << "" << 0;
    QTest::newRow("garbage after bracket") << QByteArray("[::1]x") << false << "" << 0;
    QTest::newRow("empty host") << QByteArray(":80") << false << "" << 0;
    QTest::newRow("empty") << QByteArray() << false << "" << 0;
}

void TestUtilities::parseAuthority()
{
    QFETCH(QByteArray, authority);
    QFETCH(bool, valid);
    QFETCH(QString, host);
    QFETCH(int, port);

    QString parsedHost;
    quint16 parsedPort = 0;

    QCOMPARE(Utilities::parseAuthority(authority, 80, parsedHost, parsedPort), valid);

    if (valid)
    {
        QCOMPARE(parsedHost, host);
        QCOMPARE(parsedPort, quint16(port));
    }
}

void TestUtilities::hostIsExposed_data()
{
    QTest::addColumn<QString>("host");
    QTest::addColumn<bool>("expected");

    QTest::newRow("empty (localhost by default)") << "" << false;
    QTest::newRow("spaces") << "  " << false;
    QTest::newRow("localhost") << "localhost" << false;
    QTest::newRow("localhost in other case") << "LocalHost" << false;
    QTest::newRow("ipv4 loopback") << "127.0.0.1" << false;
    QTest::newRow("other ipv4 loopback") << "127.0.0.2" << false;
    QTest::newRow("ipv6 loopback") << "::1" << false;
    QTest::newRow("ipv6 loopback in brackets") << "[::1]" << false;

    QTest::newRow("any ipv4") << "0.0.0.0" << true;
    QTest::newRow("any ipv6") << "[::]" << true;
    QTest::newRow("lan address") << "192.168.1.5" << true;
    QTest::newRow("host name") << "myhost.local" << true;
}

void TestUtilities::hostIsExposed()
{
    QFETCH(QString, host);
    QFETCH(bool, expected);

    QCOMPARE(Utilities::hostIsExposed(host), expected);
}

void TestUtilities::getProxyConnectionHost_data()
{
    QTest::addColumn<QString>("host");
    QTest::addColumn<QString>("expected");

    QTest::newRow("localhost") << "localhost" << "localhost";
    QTest::newRow("empty") << "" << "localhost";
    QTest::newRow("any ipv4") << "0.0.0.0" << "localhost";
    QTest::newRow("any ipv6") << "::" << "localhost";
    QTest::newRow("any ipv6 in brackets") << "[::]" << "localhost";
    QTest::newRow("ipv4") << "192.168.1.5" << "192.168.1.5";
    QTest::newRow("ipv6") << "::1" << "::1";
    QTest::newRow("ipv6 in brackets") << "[::1]" << "::1";
    QTest::newRow("host name") << "proxy.local" << "proxy.local";
}

void TestUtilities::getProxyConnectionHost()
{
    QFETCH(QString, host);
    QFETCH(QString, expected);

    QCOMPARE(Utilities::getProxyConnectionHost(host), expected);
}

void TestUtilities::formatProxyUrl_data()
{
    QTest::addColumn<QString>("host");
    QTest::addColumn<QString>("expected");

    QTest::newRow("localhost") << "localhost" << "socks5h://localhost:1080";
    QTest::newRow("empty") << "" << "socks5h://localhost:1080";
    QTest::newRow("any ipv4") << "0.0.0.0" << "socks5h://localhost:1080";
    QTest::newRow("any ipv6") << "::" << "socks5h://localhost:1080";
    QTest::newRow("any ipv6 in brackets") << "[::]" << "socks5h://localhost:1080";
    QTest::newRow("ipv4") << "192.168.1.5" << "socks5h://192.168.1.5:1080";
    QTest::newRow("ipv6") << "::1" << "socks5h://[::1]:1080";
    QTest::newRow("ipv6 in brackets") << "[::1]" << "socks5h://[::1]:1080";
    QTest::newRow("host name") << "proxy.local" << "socks5h://proxy.local:1080";
}

void TestUtilities::formatProxyUrl()
{
    QFETCH(QString, host);
    QFETCH(QString, expected);

    QCOMPARE(Utilities::formatProxyUrl("socks5h", host, 1080), expected);
}

void TestUtilities::proxyAddressesConflict_data()
{
    QTest::addColumn<QString>("firstHost");
    QTest::addColumn<int>("firstPort");
    QTest::addColumn<QString>("secondHost");
    QTest::addColumn<int>("secondPort");
    QTest::addColumn<bool>("expected");

    QTest::newRow("defaults") << "localhost" << 1080 << "localhost" << 8080 << false;
    QTest::newRow("same address and port") << "localhost" << 1080 << "localhost" << 1080 << true;
    QTest::newRow("empty means localhost") << "" << 1080 << "localhost" << 1080 << true;
    QTest::newRow("localhost in other case") << "LOCALHOST" << 1080 << "localhost" << 1080 << true;
    QTest::newRow("localhost and ipv4 loopback") << "localhost" << 1080 << "127.0.0.1" << 1080 << true;
    QTest::newRow("localhost and ipv6 loopback") << "localhost" << 1080 << "[::1]" << 1080 << true;
    QTest::newRow("any ipv4 includes others") << "0.0.0.0" << 1080 << "192.168.1.5" << 1080 << true;
    QTest::newRow("any ipv4 includes localhost") << "localhost" << 1080 << "0.0.0.0" << 1080 << true;
    QTest::newRow("any ipv6 includes others") << "192.168.1.5" << 1080 << "[::]" << 1080 << true;
    QTest::newRow("any address with other port") << "0.0.0.0" << 1080 << "0.0.0.0" << 8080 << false;
    QTest::newRow("different addresses") << "192.168.1.5" << 1080 << "192.168.1.6" << 1080 << false;
    QTest::newRow("loopback and other address") << "localhost" << 1080 << "192.168.1.5" << 1080 << false;
    QTest::newRow("same host name") << "proxy.local" << 1080 << "PROXY.local" << 1080 << true;
}

void TestUtilities::proxyAddressesConflict()
{
    QFETCH(QString, firstHost);
    QFETCH(int, firstPort);
    QFETCH(QString, secondHost);
    QFETCH(int, secondPort);
    QFETCH(bool, expected);

    QCOMPARE(Utilities::proxyAddressesConflict(firstHost, firstPort, secondHost, secondPort), expected);
    QCOMPARE(Utilities::proxyAddressesConflict(secondHost, secondPort, firstHost, firstPort), expected);
}

void TestUtilities::formatUptime_data()
{
    QTest::addColumn<qint64>("milliseconds");
    QTest::addColumn<QString>("expected");

    static const qint64 Minute = 60000;

    QTest::newRow("start") << qint64(0) << "";
    QTest::newRow("less than a minute") << Minute - 1 << "";
    QTest::newRow("a minute") << Minute << "1 min";
    QTest::newRow("less than an hour") << 60 * Minute - 1 << "59 min";
    QTest::newRow("an hour") << 60 * Minute << "1 h 0 min";
    QTest::newRow("hours") << 135 * Minute << "2 h 15 min";
    QTest::newRow("less than a day") << 24 * 60 * Minute - 1 << "23 h 59 min";
    QTest::newRow("a day") << 24 * 60 * Minute << "1 d 0 h";
    QTest::newRow("days") << (3 * 24 + 5) * 60 * Minute + 30 * Minute << "3 d 5 h";
}

void TestUtilities::formatUptime()
{
    QFETCH(qint64, milliseconds);
    QFETCH(QString, expected);

    QCOMPARE(Utilities::formatUptime(milliseconds), expected);
}

void TestUtilities::describeSshError_data()
{
    QTest::addColumn<QByteArray>("output");
    QTest::addColumn<QString>("expected");

    QTest::newRow("authentication")
            << QByteArray("tunnel@192.0.2.1: Permission denied (publickey).")
            << "Authentication failed: the server rejected the private key";
    QTest::newRow("host key")
            << QByteArray("No ED25519 host key is known for 192.0.2.1.\r\nHost key verification failed.")
            << "Server identity verification failed, run Trust Check in Preferences";
    QTest::newRow("resolve")
            << QByteArray("ssh: Could not resolve hostname nonexistent.invalid: Name or service not known")
            << "Unable to resolve the server address";
    QTest::newRow("refused")
            << QByteArray("ssh: connect to host 127.0.0.1 port 1: Connection refused")
            << "Connection refused by the server";
    QTest::newRow("timeout")
            << QByteArray("ssh: connect to host 192.0.2.1 port 22: Connection timed out")
            << "Connection to the server timed out";
    QTest::newRow("local port in use")
            << QByteArray("bind [127.0.0.1]:1080: Address already in use\n"
                          "channel_setup_fwd_listener_tcpip: cannot listen to port: 1080\n"
                          "Could not request local forwarding.")
            << "Local SOCKS5 proxy port is already in use";
    QTest::newRow("server alive timeout")
            << QByteArray("Timeout, server 192.0.2.1 not responding.")
            << "Server stopped responding";
    QTest::newRow("closed by server")
            << QByteArray("Connection to 192.0.2.1 closed by remote host.")
            << "Connection was closed by the server";
    QTest::newRow("unknown error uses last line")
            << QByteArray("debug noise\nSomething unexpected happened\n")
            << "Something unexpected happened";
    QTest::newRow("empty") << QByteArray() << "";
}

void TestUtilities::describeSshError()
{
    QFETCH(QByteArray, output);
    QFETCH(QString, expected);

    QCOMPARE(Utilities::describeSshError(output), expected);
}

void TestUtilities::summarizeSshError_data()
{
    QTest::addColumn<QByteArray>("output");
    QTest::addColumn<QString>("expected");

    QTest::newRow("authentication")
            << QByteArray("tunnel@192.0.2.1: Permission denied (publickey).")
            << "Authentication failed";
    QTest::newRow("host key")
            << QByteArray("Host key verification failed.")
            << "Server identity not verified";
    QTest::newRow("resolve")
            << QByteArray("ssh: Could not resolve hostname nonexistent.invalid: Name or service not known")
            << "Server address not found";
    QTest::newRow("refused")
            << QByteArray("ssh: connect to host 127.0.0.1 port 1: Connection refused")
            << "Server unreachable";
    QTest::newRow("timeout")
            << QByteArray("ssh: connect to host 192.0.2.1 port 22: Connection timed out")
            << "Server unreachable";
    QTest::newRow("no route")
            << QByteArray("ssh: connect to host 192.0.2.1 port 22: No route to host")
            << "Server unreachable";
    QTest::newRow("local port in use")
            << QByteArray("bind [127.0.0.1]:1080: Address already in use")
            << "Proxy port is busy";
    QTest::newRow("server alive timeout")
            << QByteArray("Timeout, server 192.0.2.1 not responding.")
            << "Connection lost";
    QTest::newRow("closed by server")
            << QByteArray("Connection to 192.0.2.1 closed by remote host.")
            << "Connection lost";
    QTest::newRow("unknown error doesn't expose ssh output")
            << QByteArray("debug noise\nSomething unexpected happened\n")
            << "Connection failed";
    QTest::newRow("empty") << QByteArray() << "";
}

void TestUtilities::summarizeSshError()
{
    QFETCH(QByteArray, output);
    QFETCH(QString, expected);

    QCOMPARE(Utilities::summarizeSshError(output), expected);
}

void TestUtilities::sshPrivateKeyLooksValid_data()
{
    QTest::addColumn<QString>("privateKey");
    QTest::addColumn<bool>("expected");

    QTest::newRow("openssh") << "-----BEGIN OPENSSH PRIVATE KEY-----\nb3BlbnNzaA==\n-----END OPENSSH PRIVATE KEY-----"
                             << true;
    QTest::newRow("rsa") << "-----BEGIN RSA PRIVATE KEY-----\nMIIEow==\n-----END RSA PRIVATE KEY-----" << true;
    QTest::newRow("final line break") << "-----BEGIN OPENSSH PRIVATE KEY-----\nb3BlbnNzaA==\n-----END OPENSSH PRIVATE KEY-----\n"
                                      << true;
    QTest::newRow("crlf") << "-----BEGIN OPENSSH PRIVATE KEY-----\r\nb3BlbnNzaA==\r\n-----END OPENSSH PRIVATE KEY-----\r\n"
                          << true;
    QTest::newRow("surrounding whitespace") << "\n  -----BEGIN OPENSSH PRIVATE KEY-----\nb3BlbnNzaA==\n-----END OPENSSH PRIVATE KEY-----  \n\n"
                                            << true;
    QTest::newRow("text after the key") << "-----BEGIN OPENSSH PRIVATE KEY-----\nb3BlbnNzaA==\n-----END OPENSSH PRIVATE KEY-----\nmore"
                                        << false;
    QTest::newRow("public key") << "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIOZWfDXuZUACUxEdbYQYIdIcF1/+5hhtVwvnZ6AqraZH"
                                << false;
    QTest::newRow("no body") << "-----BEGIN OPENSSH PRIVATE KEY-----\n-----END OPENSSH PRIVATE KEY-----" << false;
    QTest::newRow("too large") << "-----BEGIN OPENSSH PRIVATE KEY-----\n" + QString(40000, 'A') +
                                  "\n-----END OPENSSH PRIVATE KEY-----" << false;
    QTest::newRow("empty") << "" << false;
}

void TestUtilities::sshPrivateKeyLooksValid()
{
    QFETCH(QString, privateKey);
    QFETCH(bool, expected);

    QCOMPARE(Utilities::sshPrivateKeyLooksValid(privateKey), expected);
}

void TestUtilities::sshPrivateKeyIsEncrypted_data()
{
    QTest::addColumn<QString>("privateKey");
    QTest::addColumn<bool>("expected");

    auto openSshKey = [](const QByteArray& data) {
        return QString("-----BEGIN OPENSSH PRIVATE KEY-----\n%1\n-----END OPENSSH PRIVATE KEY-----\n")
            .arg(QString::fromLatin1(data.toBase64()));
    };

    QByteArray signature("openssh-key-v1\0", 15);

    QTest::newRow("openssh without passphrase") << openSshKey(signature + QByteArray("\0\0\0\4none\0\0\0\4none", 16)) << false;
    QTest::newRow("openssh with passphrase") << openSshKey(signature + QByteArray("\0\0\0\12aes256-ctr", 14)) << true;
    QTest::newRow("not openssh format") << openSshKey("something else") << false;
    QTest::newRow("legacy pem without passphrase") << "-----BEGIN RSA PRIVATE KEY-----\nMIIEow==\n-----END RSA PRIVATE KEY-----" << false;
    QTest::newRow("legacy pem with passphrase") << "-----BEGIN RSA PRIVATE KEY-----\nProc-Type: 4,ENCRYPTED\n"
                                                   "DEK-Info: AES-128-CBC,00\n\nMIIEow==\n-----END RSA PRIVATE KEY-----" << true;
    QTest::newRow("pkcs8 with passphrase") << "-----BEGIN ENCRYPTED PRIVATE KEY-----\nMIIEow==\n-----END ENCRYPTED PRIVATE KEY-----" << true;
    QTest::newRow("empty") << "" << false;
}

void TestUtilities::sshPrivateKeyIsEncrypted()
{
    QFETCH(QString, privateKey);
    QFETCH(bool, expected);

    QCOMPARE(Utilities::sshPrivateKeyIsEncrypted(privateKey), expected);
}

void TestUtilities::generatedKeysEncryption_data()
{
    QTest::addColumn<QString>("format");
    QTest::addColumn<QString>("passphrase");

    QTest::newRow("openssh without passphrase") << "RFC4716" << "";
    QTest::newRow("openssh with passphrase") << "RFC4716" << "secret";
    QTest::newRow("legacy pem without passphrase") << "PEM" << "";
    QTest::newRow("legacy pem with passphrase") << "PEM" << "secret";
    QTest::newRow("pkcs8 with passphrase") << "PKCS8" << "secret";
}

// Keys of all formats as they are actually written by ssh-keygen
void TestUtilities::generatedKeysEncryption()
{
    QFETCH(QString, format);
    QFETCH(QString, passphrase);

    if (QStandardPaths::findExecutable("ssh-keygen").isEmpty())
    {
        QSKIP("ssh-keygen is not installed");
    }

    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    QString path = directory.filePath("Key");

    QProcess process;
    process.start("ssh-keygen", {"-t", "rsa", "-b", "2048", "-m", format, "-N", passphrase, "-f", path, "-q"});
    QVERIFY(process.waitForFinished(30000));
    QCOMPARE(process.exitCode(), 0);

    QFile file(path);
    QVERIFY(file.open(QIODevice::ReadOnly));

    QString privateKey = QString::fromUtf8(file.readAll());

    QVERIFY(Utilities::sshPrivateKeyLooksValid(privateKey));
    QCOMPARE(Utilities::sshPrivateKeyIsEncrypted(privateKey), !passphrase.isEmpty());
}

void TestUtilities::getKeyTypeLabel_data()
{
    QTest::addColumn<QString>("keyType");
    QTest::addColumn<QString>("expected");

    QTest::newRow("ed25519") << "ssh-ed25519" << "ED25519";
    QTest::newRow("ecdsa") << "ecdsa-sha2-nistp256" << "ECDSA";
    QTest::newRow("rsa in other case") << "SSH-RSA" << "RSA";
    QTest::newRow("unknown") << "ssh-future" << "SSH-FUTURE";
}

void TestUtilities::getKeyTypeLabel()
{
    QFETCH(QString, keyType);
    QFETCH(QString, expected);

    QCOMPARE(Utilities::getKeyTypeLabel(keyType), expected);
}

void TestUtilities::getKeyTypeAndFingerprint()
{
    // Expected fingerprint is taken from "ssh-keygen -lf" output for the same key
    QString hostRecord("192.0.2.1 ssh-ed25519 "
                       "AAAAC3NzaC1lZDI1NTE5AAAAIOZWfDXuZUACUxEdbYQYIdIcF1/+5hhtVwvnZ6AqraZH");

    auto [keyType, fingerprint] = Utilities::getKeyTypeAndFingerprint(hostRecord);

    QCOMPARE(keyType, QString("ssh-ed25519"));
    QCOMPARE(fingerprint, QString("n9zl8+YNkmP5rLVUuGkf2vkQ6fwDALdhTzHK6vBovkA"));
}

void TestUtilities::getMissingSshToolMessage()
{
    QString message = Utilities::getMissingSshToolMessage("shprot-nonexistent-tool");
    QVERIFY(message.contains("shprot-nonexistent-tool"));

    if (QStandardPaths::findExecutable("ssh").isEmpty())
    {
        QSKIP("OpenSSH client is not installed");
    }

    QVERIFY(Utilities::getMissingSshToolMessage("ssh").isEmpty());
}

void TestUtilities::writePrivateKeyFile()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    QString path = directory.filePath("Identity");
    QByteArray privateKey("-----BEGIN OPENSSH PRIVATE KEY-----\nb3BlbnNzaA==\n-----END OPENSSH PRIVATE KEY-----\n");

    // An existing file is replaced, so permissions of a new file are applied
    QVERIFY(Utilities::writePrivateKeyFile(path, "old content"));
    QVERIFY(Utilities::writePrivateKeyFile(path, privateKey));

    QFile file(path);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QCOMPARE(file.readAll(), privateKey);
    file.close();

    // OpenSSH doesn't accept these forms of a key, so they are normalized
    for (const QByteArray& untidyPrivateKey : {
             QByteArray("-----BEGIN OPENSSH PRIVATE KEY-----\nb3BlbnNzaA==\n-----END OPENSSH PRIVATE KEY-----"),
             QByteArray("-----BEGIN OPENSSH PRIVATE KEY-----\r\nb3BlbnNzaA==\r\n-----END OPENSSH PRIVATE KEY-----\r\n"),
             QByteArray("\n -----BEGIN OPENSSH PRIVATE KEY-----\nb3BlbnNzaA==\n-----END OPENSSH PRIVATE KEY----- \n\n")})
    {
        QVERIFY(Utilities::writePrivateKeyFile(path, untidyPrivateKey));
        QVERIFY(file.open(QIODevice::ReadOnly));
        QCOMPARE(file.readAll(), privateKey);
        file.close();
    }

#ifndef Q_OS_WIN
    // On Windows the file inherits the profile directory ACL, which QFile::permissions() doesn't reflect
    QFileDevice::Permissions others = QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ExeGroup |
                                      QFileDevice::ReadOther | QFileDevice::WriteOther | QFileDevice::ExeOther;
    QCOMPARE(file.permissions() & others, QFileDevice::Permissions());
#endif
}

void TestUtilities::generateSshKeys()
{
    if (QStandardPaths::findExecutable("ssh-keygen").isEmpty())
    {
        QSKIP("ssh-keygen is not installed");
    }

    QString privateKey = Utilities::generateSshPrivateKey();
    QVERIFY(Utilities::sshPrivateKeyLooksValid(privateKey.trimmed()));

    QString publicKey = Utilities::getSshPublicKey(privateKey);
    QVERIFY2(publicKey.startsWith("ssh-ed25519 "), qPrintable(publicKey));

    // Key damaged by copying still works, ssh-keygen loads it the same way ssh does
    QCOMPARE(Utilities::getSshPublicKey(privateKey.trimmed()), publicKey);
    QCOMPARE(Utilities::getSshPublicKey(QString(privateKey).replace("\n", "\r\n")), publicKey);

    // Nothing to copy from a key that can't be loaded
    QVERIFY(Utilities::getSshPublicKey("-----BEGIN OPENSSH PRIVATE KEY-----\nb3BlbnNzaA==\n-----END OPENSSH PRIVATE KEY-----").isEmpty());
    QVERIFY(Utilities::getSshPublicKey("").isEmpty());
}

void TestUtilities::getSshKeyInstallCommand_data()
{
    QTest::addColumn<QString>("comment");

    QTest::newRow("generated") << "user@host/Shprot";
    QTest::newRow("single quote") << "it's mine";
    QTest::newRow("double quotes") << R"("mine")";
    QTest::newRow("backslash") << R"(C:\new\table)";
    QTest::newRow("percent") << "100% %s %1";
    QTest::newRow("substitution") << "$(touch injected) `touch injected`";
    QTest::newRow("command separator") << "'; touch injected; echo '";
}

// The command is really executed the way it's done on a server, a comment must not be able to break out of it
void TestUtilities::getSshKeyInstallCommand()
{
    QFETCH(QString, comment);

#ifdef Q_OS_WIN
    QSKIP("POSIX shell is not available");
#endif

    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    QString publicKey = "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIOZWfDXuZUACUxEdbYQYIdIcF1/+5hhtVwvnZ6AqraZH " + comment;

    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    environment.insert("HOME", directory.path());

    QProcess process;
    process.setProcessEnvironment(environment);
    process.setWorkingDirectory(directory.path());

    // Running it again adds the same line, the rest of the file stays
    for (int i = 0; i < 2; i++)
    {
        process.start("sh", {"-c", Utilities::getSshKeyInstallCommand(publicKey)});
        QVERIFY(process.waitForFinished(5000));
        QVERIFY2(process.exitCode() == 0, process.readAllStandardError().constData());
    }

    QVERIFY(!QFile::exists(directory.filePath("injected")));

    QString authorizedKeysPath = directory.filePath(".ssh/authorized_keys");
    QFile authorizedKeys(authorizedKeysPath);
    QVERIFY(authorizedKeys.open(QIODevice::ReadOnly));

    QString line = QString(R"(restrict,port-forwarding,command="false" %1)").arg(publicKey) + "\n";
    QCOMPARE(QString::fromUtf8(authorizedKeys.readAll()), line + line);

    QFileDevice::Permissions others = QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ExeGroup |
                                      QFileDevice::ReadOther | QFileDevice::WriteOther | QFileDevice::ExeOther;
    QCOMPARE(authorizedKeys.permissions() & others, QFileDevice::Permissions());

    if (QStandardPaths::findExecutable("ssh-keygen").isEmpty())
    {
        QSKIP("ssh-keygen is not installed");
    }

    // OpenSSH parses the line as a key with options. Expected fingerprint is the one from getKeyTypeAndFingerprint test.
    process.start("ssh-keygen", {"-l", "-f", authorizedKeysPath});
    QVERIFY(process.waitForFinished(5000));
    QCOMPARE(process.exitCode(), 0);
    QVERIFY(process.readAllStandardOutput().contains("SHA256:n9zl8+YNkmP5rLVUuGkf2vkQ6fwDALdhTzHK6vBovkA"));
}

QTEST_GUILESS_MAIN(TestUtilities)

#include "TestUtilities.moc"
