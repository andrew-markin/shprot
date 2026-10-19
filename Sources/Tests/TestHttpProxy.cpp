#include <QRegularExpression>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTest>

#include "HttpProxyServer.h"

// HTTP proxy is tested end to end: client -> proxy (without SOCKS) -> origin server, all on loopback
class TestHttpProxy : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void absoluteFormIsRewritten();
    void requestBodyIsForwarded();
    void hostHeaderIsAdded();
    void upgradeIsPreserved();
    void connectTunnel();
    void connectTunnelIpv6();

    void invalidRequest_data();
    void invalidRequest();
    void tooLargeHeaders();
    void unreachableTarget();

private:
    void serveOrigin(QTcpServer* server);
    QByteArray exchange(const QByteArray& request);
    QByteArray withOriginPort(const char* request) const;

private:
    HttpProxyServer* m_proxy;
    QTcpServer* m_origin;
    QList<QByteArray> m_originRequests;
};

void TestHttpProxy::initTestCase()
{
    m_origin = new QTcpServer(this);
    QVERIFY(m_origin->listen(QHostAddress::LocalHost));
    serveOrigin(m_origin);

    m_proxy = new HttpProxyServer(this);
    QVERIFY(m_proxy->start("localhost", 0, QNetworkProxy(QNetworkProxy::NoProxy)));
}

void TestHttpProxy::init()
{
    m_originRequests.clear();
}

void TestHttpProxy::absoluteFormIsRewritten()
{
    QByteArray response = exchange(withOriginPort("GET http://127.0.0.1:%1/path?q=1 HTTP/1.1\r\n"
                                                  "Host: 127.0.0.1:%1\r\n"
                                                  "Proxy-Authorization: Basic c2VjcmV0\r\n"
                                                  "Proxy-Connection: keep-alive\r\n"
                                                  "Connection: keep-alive\r\n"
                                                  "Keep-Alive: 300\r\n"
                                                  "Accept: */*\r\n"
                                                  "\r\n"));

    QVERIFY2(response.startsWith("HTTP/1.1 200 OK\r\n"), response.constData());
    QVERIFY(response.endsWith("ok"));
    QCOMPARE(m_originRequests.size(), 1);

    const QByteArray& request = m_originRequests.first();
    QVERIFY2(request.startsWith("GET /path?q=1 HTTP/1.1\r\n"), request.constData());
    QVERIFY(request.contains("\r\nAccept: */*\r\n"));
    QVERIFY(request.contains("\r\nConnection: close\r\n"));
    QVERIFY(!request.contains("Proxy-Authorization"));
    QVERIFY(!request.contains("Proxy-Connection"));
    QVERIFY(!request.contains("Keep-Alive"));
    QVERIFY(!request.contains("keep-alive"));
}

void TestHttpProxy::requestBodyIsForwarded()
{
    QByteArray response = exchange(withOriginPort("PATCH http://127.0.0.1:%1/items HTTP/1.1\r\n"
                                                  "Host: 127.0.0.1:%1\r\n"
                                                  "Content-Length: 7\r\n"
                                                  "\r\n"
                                                  "payload"));

    QVERIFY2(response.startsWith("HTTP/1.1 200 OK\r\n"), response.constData());
    QCOMPARE(m_originRequests.size(), 1);
    QVERIFY(m_originRequests.first().startsWith("PATCH /items HTTP/1.1\r\n"));
    QVERIFY(m_originRequests.first().endsWith("\r\n\r\npayload"));
}

void TestHttpProxy::hostHeaderIsAdded()
{
    exchange(withOriginPort("GET http://127.0.0.1:%1 HTTP/1.0\r\n\r\n"));

    QCOMPARE(m_originRequests.size(), 1);
    QVERIFY(m_originRequests.first().startsWith("GET / HTTP/1.0\r\n"));
    QVERIFY(m_originRequests.first().contains(withOriginPort("\r\nHost: 127.0.0.1:%1\r\n")));
}

void TestHttpProxy::upgradeIsPreserved()
{
    exchange(withOriginPort("GET http://127.0.0.1:%1/socket HTTP/1.1\r\n"
                            "Host: 127.0.0.1:%1\r\n"
                            "Connection: Upgrade\r\n"
                            "Upgrade: websocket\r\n"
                            "\r\n"));

    QCOMPARE(m_originRequests.size(), 1);
    QVERIFY(m_originRequests.first().contains("\r\nConnection: Upgrade\r\n"));
    QVERIFY(!m_originRequests.first().contains("Connection: close"));
}

void TestHttpProxy::connectTunnel()
{
    // Data sent right after CONNECT (before the proxy answers) must reach the target too
    QByteArray response = exchange(withOriginPort("CONNECT 127.0.0.1:%1 HTTP/1.1\r\n"
                                                  "Host: 127.0.0.1:%1\r\n"
                                                  "\r\n"
                                                  "GET /tunneled HTTP/1.1\r\n"
                                                  "Host: example.com\r\n"
                                                  "\r\n"));

    QVERIFY2(response.startsWith("HTTP/1.1 200 Connection Established\r\n\r\nHTTP/1.1 200 OK\r\n"),
             response.constData());
    QCOMPARE(m_originRequests.size(), 1);
    QVERIFY(m_originRequests.first().startsWith("GET /tunneled HTTP/1.1\r\n"));
}

void TestHttpProxy::connectTunnelIpv6()
{
    QTcpServer originIpv6;

    if (!originIpv6.listen(QHostAddress::LocalHostIPv6))
    {
        QSKIP("IPv6 loopback is not available");
    }

    serveOrigin(&originIpv6);

    QByteArray response = exchange(QString("CONNECT [::1]:%1 HTTP/1.1\r\n"
                                           "\r\n"
                                           "GET /ipv6 HTTP/1.1\r\n"
                                           "Host: example.com\r\n"
                                           "\r\n").arg(originIpv6.serverPort()).toLatin1());

    QVERIFY2(response.startsWith("HTTP/1.1 200 Connection Established\r\n"), response.constData());
    QCOMPARE(m_originRequests.size(), 1);
    QVERIFY(m_originRequests.first().startsWith("GET /ipv6 HTTP/1.1\r\n"));
}

void TestHttpProxy::invalidRequest_data()
{
    QTest::addColumn<QByteArray>("request");

    QTest::newRow("garbage") << QByteArray("GARBAGE\r\n\r\n");
    QTest::newRow("origin form") << QByteArray("GET /path HTTP/1.1\r\nHost: example.com\r\n\r\n");
    QTest::newRow("https in absolute form") << QByteArray("GET https://example.com/ HTTP/1.1\r\n\r\n");
    QTest::newRow("not http version") << QByteArray("GET http://example.com/ FTP/1.0\r\n\r\n");
    QTest::newRow("ipv6 without brackets") << QByteArray("CONNECT ::1:443 HTTP/1.1\r\n\r\n");
    QTest::newRow("header without colon") << QByteArray("GET http://example.com/ HTTP/1.1\r\nBroken\r\n\r\n");
}

void TestHttpProxy::invalidRequest()
{
    QFETCH(QByteArray, request);

    QByteArray response = exchange(request);

    QVERIFY2(response.startsWith("HTTP/1.1 400 Bad Request\r\n"), response.constData());
    QVERIFY(m_originRequests.isEmpty());
}

void TestHttpProxy::tooLargeHeaders()
{
    QByteArray request = withOriginPort("GET http://127.0.0.1:%1/ HTTP/1.1\r\nX-Padding: ");
    request.append(QByteArray(70000, 'a'));

    QByteArray response = exchange(request);

    QVERIFY2(response.startsWith("HTTP/1.1 431 Request Header Fields Too Large\r\n"), response.constData());
    QVERIFY(m_originRequests.isEmpty());
}

void TestHttpProxy::unreachableTarget()
{
    // Take a free port and release it, so nothing listens there
    QTcpServer closedServer;
    QVERIFY(closedServer.listen(QHostAddress::LocalHost));
    quint16 closedPort = closedServer.serverPort();
    closedServer.close();

    QByteArray response = exchange(QString("GET http://127.0.0.1:%1/ HTTP/1.1\r\n\r\n").arg(closedPort).toLatin1());

    QVERIFY2(response.startsWith("HTTP/1.1 502 Bad Gateway\r\n"), response.constData());
}

// Origin server answers "ok" once the whole request (with Content-Length body) is received
void TestHttpProxy::serveOrigin(QTcpServer* server)
{
    connect(server, &QTcpServer::newConnection, this, [this, server]() {
        while (QTcpSocket* socket = server->nextPendingConnection())
        {
            connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
            connect(socket, &QTcpSocket::readyRead, socket, [this, socket]() {
                QByteArray request = socket->property("request").toByteArray() + socket->readAll();
                socket->setProperty("request", request);

                qsizetype headersEnd = request.indexOf("\r\n\r\n");

                if (headersEnd < 0)
                {
                    return;
                }

                static const QRegularExpression ContentLengthRegEx(R"(\r\ncontent-length: *(\d+))",
                                                                   QRegularExpression::CaseInsensitiveOption);

                QRegularExpressionMatch match = ContentLengthRegEx.match(QString::fromLatin1(request.left(headersEnd)));
                qsizetype bodySize = match.hasMatch() ? match.captured(1).toLongLong() : 0;

                if (request.size() < headersEnd + 4 + bodySize)
                {
                    return;
                }

                m_originRequests.append(request);
                socket->write("HTTP/1.1 200 OK\r\nContent-Length: 2\r\nConnection: close\r\n\r\nok");
                socket->disconnectFromHost();
            });
        }
    });
}

// Sends the request through the proxy and returns everything received until the proxy disconnects
QByteArray TestHttpProxy::exchange(const QByteArray& request)
{
    QTcpSocket client;
    QByteArray response;

    connect(&client, &QTcpSocket::readyRead, &client, [&client, &response]() {
        response.append(client.readAll());
    });

    client.connectToHost(QHostAddress::LocalHost, m_proxy->serverPort());

    if (!QTest::qWaitFor([&client]() { return client.state() == QAbstractSocket::ConnectedState; }, 5000))
    {
        return QByteArray("Unable to connect to the proxy");
    }

    client.write(request);

    if (!QTest::qWaitFor([&client]() { return client.state() == QAbstractSocket::UnconnectedState; }, 5000))
    {
        response.append("\n(proxy did not close the connection)");
    }

    response.append(client.readAll());
    return response;
}

QByteArray TestHttpProxy::withOriginPort(const char* request) const
{
    return QString::fromLatin1(request).arg(m_origin->serverPort()).toLatin1();
}

QTEST_GUILESS_MAIN(TestHttpProxy)

#include "TestHttpProxy.moc"
