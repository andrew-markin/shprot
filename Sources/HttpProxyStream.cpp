#include "HttpProxyStream.h"

#include <QDebug>
#include <QRegularExpression>

HttpProxyStream::HttpProxyStream(QTcpSocket* clientSocket, const QNetworkProxy& socksProxy, QObject* parent)
    : QObject(parent), m_clientSocket(clientSocket)
{
    m_clientSocket->setParent(this);

    connect(m_clientSocket, &QTcpSocket::readyRead, this, &HttpProxyStream::readClientSocket);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &HttpProxyStream::closeAndDeleteLater);

    m_targetSocket = new QTcpSocket(this);
    m_targetSocket->setProxy(socksProxy);

    connect(m_targetSocket, &QTcpSocket::connected, this, &HttpProxyStream::finishHandshake);
    connect(m_targetSocket, &QTcpSocket::readyRead, this, &HttpProxyStream::readTargetSocket);
    connect(m_targetSocket, &QTcpSocket::disconnected, this, &HttpProxyStream::closeAndDeleteLater);
}

void HttpProxyStream::closeAndDeleteLater()
{
    m_clientSocket->disconnectFromHost();
    m_targetSocket->disconnectFromHost();
    deleteLater();
}

void HttpProxyStream::readClientSocket()
{
    if (m_handshakeFinished)
    {
        m_targetSocket->write(m_clientSocket->readAll());
        return;
    }

    m_handshakeBuffer.append(m_clientSocket->readAll());

    if (!m_handshakeBuffer.contains("\r\n\r\n"))
    {
        return;
    }

    static const QRegularExpression RequestRegEx(R"((CONNECT|GET|POST) (?:https?:\/\/)?([^:\/\s]+)(?::(\d+))?)");
    QString firstLine = QString::fromUtf8(m_handshakeBuffer.left(m_handshakeBuffer.indexOf("\r\n")));
    QRegularExpressionMatch match = RequestRegEx.match(firstLine);

    if (!match.hasMatch())
    {
        qWarning() << "Invalid HTTP request:" << firstLine;
        closeAndDeleteLater();
        return;
    }

    QString host = match.captured(2);
    QString method = match.captured(1);
    quint16 port = match.captured(3).isEmpty() ? (method == "CONNECT" ? 443 : 80) : match.captured(3).toUShort();

    m_targetSocket->connectToHost(host, port);
}

void HttpProxyStream::finishHandshake()
{
    m_handshakeFinished = true;

    if (m_handshakeBuffer.startsWith("CONNECT"))
    {
        m_clientSocket->write("HTTP/1.1 200 Connection Established\r\n\r\n");
    }
    else
    {
        m_targetSocket->write(m_handshakeBuffer);
    }

    m_handshakeBuffer.clear();
}

void HttpProxyStream::readTargetSocket()
{
    m_clientSocket->write(m_targetSocket->readAll());
}
