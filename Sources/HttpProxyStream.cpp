#include "HttpProxyStream.h"

#include <QDebug>
#include <QRegularExpression>

#include "Utilities.h"

namespace {

static const int MaxRequestHeadersSize = 65536; // 64KB
static const int MaxPendingBytes = 262144; // 256KB per direction
static const int ChunkSize = 65536; // 64KB
static const int RequestTimeout = 30000; // 30s

void forwardData(QTcpSocket* source, QTcpSocket* destination)
{
    if (destination->state() != QAbstractSocket::ConnectedState)
    {
        return;
    }

    // Stop reading when the destination is slow, so the source is throttled by TCP flow control
    while ((source->bytesAvailable() > 0) && (destination->bytesToWrite() < MaxPendingBytes))
    {
        destination->write(source->read(ChunkSize));
    }
}

} // namespace

HttpProxyStream::HttpProxyStream(QTcpSocket* clientSocket, const QNetworkProxy& socksProxy, QObject* parent)
    : QObject(parent), m_clientSocket(clientSocket)
{
    m_clientSocket->setParent(this);
    m_clientSocket->setReadBufferSize(MaxPendingBytes);

    connect(m_clientSocket, &QTcpSocket::readyRead, this, &HttpProxyStream::readRequest);
    connect(m_clientSocket, &QTcpSocket::stateChanged, this, &HttpProxyStream::pump);

    m_targetSocket = new QTcpSocket(this);
    m_targetSocket->setProxy(socksProxy);
    m_targetSocket->setReadBufferSize(MaxPendingBytes);

    connect(m_targetSocket, &QTcpSocket::connected, this, &HttpProxyStream::handleTargetConnect);
    connect(m_targetSocket, &QTcpSocket::errorOccurred, this, &HttpProxyStream::handleTargetError);
    connect(m_targetSocket, &QTcpSocket::stateChanged, this, &HttpProxyStream::pump);

    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, &HttpProxyStream::handleTimeout);
    m_timeoutTimer->start(RequestTimeout);
}

void HttpProxyStream::closeAndDeleteLater()
{
    m_state = State::Closing;
    m_clientSocket->abort();
    m_targetSocket->abort();
    pump();
}

void HttpProxyStream::readRequest()
{
    if (m_state != State::ReadingRequest)
    {
        return; // Request body or tunnel data is forwarded by pump()
    }

    m_requestBuffer.append(m_clientSocket->readAll());
    int headersEnd = m_requestBuffer.indexOf("\r\n\r\n");

    if ((headersEnd < 0) && (m_requestBuffer.size() <= MaxRequestHeadersSize))
    {
        return; // Wait for complete headers
    }

    if ((headersEnd < 0) || (headersEnd > MaxRequestHeadersSize))
    {
        qWarning() << "HTTP request headers are too large";
        respondAndClose("431 Request Header Fields Too Large");
        return;
    }

    QByteArray headers = m_requestBuffer.left(headersEnd);
    QByteArray remainder = m_requestBuffer.mid(headersEnd + 4);
    m_requestBuffer.clear();

    if (!parseRequest(headers, remainder))
    {
        qWarning() << "Invalid HTTP request:" << headers.left(headers.indexOf("\r\n"));
        respondAndClose("400 Bad Request");
        return;
    }
}

bool HttpProxyStream::parseRequest(const QByteArray& headers, const QByteArray& remainder)
{
    QList<QByteArray> lines = headers.split('\n');

    for (QByteArray& line : lines)
    {
        if (line.endsWith('\r'))
        {
            line.chop(1);
        }
    }

    QList<QByteArray> requestLine = lines.takeFirst().split(' ');

    if (requestLine.size() != 3)
    {
        return false;
    }

    static const QRegularExpression MethodRegEx(R"(^[!#$%&'*+.^_`|~0-9A-Za-z-]+$)");

    const QByteArray& method = requestLine.at(0);
    const QByteArray& target = requestLine.at(1);
    const QByteArray& version = requestLine.at(2);

    if (!MethodRegEx.match(QString::fromLatin1(method)).hasMatch() || !version.startsWith("HTTP/"))
    {
        return false;
    }

    QString host;
    quint16 port = 0;

    if (method == "CONNECT")
    {
        if (!Utilities::parseAuthority(target, 443, host, port))
        {
            return false;
        }

        m_tunnelRequested = true;
        m_requestBuffer = remainder; // E.g. TLS handshake sent optimistically
    }
    else
    {
        // Absolute form "http://authority/path?query" must be rewritten to origin form "/path?query"
        if (!target.toLower().startsWith("http://"))
        {
            return false;
        }

        QByteArray address = target.mid(7);
        qsizetype pathStart = address.indexOf('/');
        qsizetype queryStart = address.indexOf('?');

        if ((queryStart >= 0) && ((pathStart < 0) || (queryStart < pathStart)))
        {
            pathStart = queryStart;
        }

        QByteArray authority = (pathStart >= 0) ? address.left(pathStart) : address;
        QByteArray path = (pathStart >= 0) ? address.mid(pathStart) : QByteArray("/");

        if (path.startsWith('?'))
        {
            path.prepend('/');
        }

        if (!Utilities::parseAuthority(authority, 80, host, port))
        {
            return false;
        }

        QByteArrayList forwardedLines = {method + ' ' + path + ' ' + version};
        bool hostHeaderFound = false;
        bool upgradeRequested = false;

        for (const QByteArray& line : std::as_const(lines))
        {
            qsizetype colon = line.indexOf(':');

            if (colon <= 0)
            {
                return false;
            }

            QByteArray name = line.left(colon).trimmed().toLower();
            QByteArray value = line.mid(colon + 1).trimmed().toLower();

            if ((name == "proxy-connection") || (name == "proxy-authorization") || (name == "keep-alive"))
            {
                continue; // Addressed to this proxy, not to the target server
            }

            if (name == "connection")
            {
                if (!value.contains("upgrade"))
                {
                    continue; // Replaced with "Connection: close" below
                }

                upgradeRequested = true; // E.g. WebSocket, the connection becomes a tunnel after the response
            }

            if (name == "host")
            {
                hostHeaderFound = true;
            }

            forwardedLines.append(line);
        }

        if (!hostHeaderFound)
        {
            forwardedLines.append("Host: " + authority.mid(authority.lastIndexOf('@') + 1));
        }

        // Single request per connection, so the next request to another host is not sent to this one
        if (!upgradeRequested)
        {
            forwardedLines.append("Connection: close");
        }

        m_requestBuffer = forwardedLines.join("\r\n") + "\r\n\r\n" + remainder;
    }

    m_targetHost = QString("%1:%2").arg(host).arg(port).toUtf8();
    m_state = State::Connecting;
    m_timeoutTimer->start(RequestTimeout);
    m_targetSocket->connectToHost(host, port);
    return true;
}

void HttpProxyStream::handleTargetConnect()
{
    if (m_state != State::Connecting)
    {
        return;
    }

    m_state = State::Tunneling;
    m_timeoutTimer->stop();

    if (m_tunnelRequested)
    {
        m_clientSocket->write("HTTP/1.1 200 Connection Established\r\n\r\n");
    }

    m_targetSocket->write(m_requestBuffer);
    m_requestBuffer.clear();

    connect(m_clientSocket, &QTcpSocket::readyRead, this, &HttpProxyStream::pump);
    connect(m_clientSocket, &QTcpSocket::bytesWritten, this, &HttpProxyStream::pump);
    connect(m_targetSocket, &QTcpSocket::readyRead, this, &HttpProxyStream::pump);
    connect(m_targetSocket, &QTcpSocket::bytesWritten, this, &HttpProxyStream::pump);

    pump();
}

void HttpProxyStream::handleTargetError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);

    if (m_state == State::Connecting)
    {
        qWarning() << "Unable to connect to" << m_targetHost << "through the tunnel:"
                   << m_targetSocket->errorString();
        respondAndClose("502 Bad Gateway");
    }
}

void HttpProxyStream::handleTimeout()
{
    if (m_state == State::ReadingRequest)
    {
        respondAndClose("408 Request Timeout");
    }
    else if (m_state == State::Connecting)
    {
        qWarning() << "Connection to" << m_targetHost << "through the tunnel timed out";
        respondAndClose("504 Gateway Timeout");
    }
}

void HttpProxyStream::respondAndClose(const QByteArray& status)
{
    m_state = State::Closing;
    m_timeoutTimer->stop();
    m_targetSocket->abort();

    if (m_clientSocket->state() == QAbstractSocket::ConnectedState)
    {
        m_clientSocket->write("HTTP/1.1 " + status + "\r\nContent-Length: 0\r\nConnection: close\r\n\r\n");
        m_clientSocket->disconnectFromHost(); // Pending data is sent before disconnection
    }

    pump();
}

void HttpProxyStream::pump()
{
    QAbstractSocket::SocketState clientState = m_clientSocket->state();
    QAbstractSocket::SocketState targetState = m_targetSocket->state();

    if (m_state == State::Tunneling)
    {
        forwardData(m_clientSocket, m_targetSocket);
        forwardData(m_targetSocket, m_clientSocket);

        // Close the other side once everything received from the disconnected side is delivered
        if ((clientState == QAbstractSocket::UnconnectedState) && (m_clientSocket->bytesAvailable() == 0) &&
            (targetState == QAbstractSocket::ConnectedState))
        {
            m_targetSocket->disconnectFromHost();
        }

        if ((targetState == QAbstractSocket::UnconnectedState) && (m_targetSocket->bytesAvailable() == 0) &&
            (clientState == QAbstractSocket::ConnectedState))
        {
            m_clientSocket->disconnectFromHost();
        }
    }
    else if ((m_state != State::Closing) && (clientState == QAbstractSocket::UnconnectedState))
    {
        m_state = State::Closing; // Client is gone before the tunnel was established
        m_timeoutTimer->stop();
        m_targetSocket->abort();
    }

    if ((m_clientSocket->state() == QAbstractSocket::UnconnectedState) &&
        (m_targetSocket->state() == QAbstractSocket::UnconnectedState) && !m_deleteScheduled)
    {
        m_deleteScheduled = true;
        deleteLater();
    }
}
