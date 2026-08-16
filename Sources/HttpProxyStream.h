#ifndef HTTPPROXYSTREAM_H
#define HTTPPROXYSTREAM_H

#include <QNetworkProxy>
#include <QTcpSocket>

class HttpProxyStream : public QObject
{
    Q_OBJECT

public:
    explicit HttpProxyStream(QTcpSocket* clientSocket, const QNetworkProxy& socksProxy, QObject* parent = nullptr);

public slots:
    void closeAndDeleteLater();

private slots:
    void readClientSocket();
    void finishHandshake();
    void readTargetSocket();

private:
    QTcpSocket* m_clientSocket;
    QTcpSocket* m_targetSocket;
    QNetworkProxy m_socksProxy;
    QByteArray m_handshakeBuffer;
    bool m_handshakeFinished = false;
};

#endif // HTTPPROXYSTREAM_H
