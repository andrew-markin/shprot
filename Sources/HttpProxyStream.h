#ifndef HTTPPROXYSTREAM_H
#define HTTPPROXYSTREAM_H

#include <QNetworkProxy>
#include <QTcpSocket>
#include <QTimer>

class HttpProxyStream : public QObject
{
    Q_OBJECT

public:
    explicit HttpProxyStream(QTcpSocket* clientSocket, const QNetworkProxy& socksProxy, QObject* parent = nullptr);

public slots:
    void closeAndDeleteLater();

private slots:
    void readRequest();
    void handleTargetConnect();
    void handleTargetError(QAbstractSocket::SocketError error);
    void handleTimeout();
    void pump();

private:
    enum class State
    {
        ReadingRequest,
        Connecting,
        Tunneling,
        Closing
    };

    bool parseRequest(const QByteArray& headers, const QByteArray& remainder);
    void respondAndClose(const QByteArray& status);

private:
    State m_state = State::ReadingRequest;
    QTcpSocket* m_clientSocket;
    QTcpSocket* m_targetSocket;
    QTimer* m_timeoutTimer;
    QByteArray m_requestBuffer;
    QByteArray m_targetHost;
    bool m_tunnelRequested = false;
    bool m_deleteScheduled = false;
};

#endif // HTTPPROXYSTREAM_H
