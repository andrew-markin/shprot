#include "HttpProxyServer.h"

#include "HttpProxyStream.h"

HttpProxyServer::HttpProxyServer(QObject* parent) : QObject(parent)
{
    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &HttpProxyServer::handleNewConnection);
}

void HttpProxyServer::start(const QString& host, quint16 port, const QNetworkProxy& socksProxy)
{
    stop();
    m_socksProxy = socksProxy;
    QHostAddress address(host.toLower() == "localhost" ? QHostAddress::LocalHost : QHostAddress(host));

    if (!m_tcpServer->listen(address, port))
    {
        qWarning() << "Unable to start HTTP proxy server:" << m_tcpServer->errorString();
    }
}

void HttpProxyServer::stop()
{
    m_tcpServer->close();

    for (HttpProxyStream* stream : m_streams)
    {
        stream->closeAndDeleteLater();
    }

    m_streams.clear();
}

void HttpProxyServer::handleNewConnection()
{
    while (m_tcpServer->hasPendingConnections())
    {
        QTcpSocket* clientSocket = m_tcpServer->nextPendingConnection();

        HttpProxyStream* stream = new HttpProxyStream(clientSocket, m_socksProxy, this);
        m_streams.append(stream);

        connect(stream, &QObject::destroyed, this, [this](QObject* object) {
            HttpProxyStream* stream = reinterpret_cast<HttpProxyStream*>(object);
            m_streams.removeOne(stream);
        });
    }
}
