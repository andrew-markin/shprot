#include <QFile>
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

    void describeSshError_data();
    void describeSshError();

    void sshPrivateKeyLooksValid_data();
    void sshPrivateKeyLooksValid();

    void getKeyTypeLabel_data();
    void getKeyTypeLabel();

    void getKeyTypeAndFingerprint();

    void getMissingSshToolMessage();

    void writePrivateKeyFile();

    void generateSshKeys();
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

void TestUtilities::sshPrivateKeyLooksValid_data()
{
    QTest::addColumn<QString>("privateKey");
    QTest::addColumn<bool>("expected");

    QTest::newRow("openssh") << "-----BEGIN OPENSSH PRIVATE KEY-----\nb3BlbnNzaA==\n-----END OPENSSH PRIVATE KEY-----"
                             << true;
    QTest::newRow("rsa") << "-----BEGIN RSA PRIVATE KEY-----\nMIIEow==\n-----END RSA PRIVATE KEY-----" << true;
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
}

QTEST_GUILESS_MAIN(TestUtilities)

#include "TestUtilities.moc"
