#ifndef HTTPPROXYSERVER_H
#define HTTPPROXYSERVER_H

#include <QList>
#include <QNetworkProxy>
#include <QTcpServer>

class HttpProxyStream;

class HttpProxyServer : public QObject
{
    Q_OBJECT

public:
    HttpProxyServer(QObject* parent = nullptr);

public slots:
    void start(const QString& host, quint16 port, const QNetworkProxy& socksProxy);
    void stop();

private slots:
    void handleNewConnection();

private:
    QTcpServer* m_tcpServer;
    QNetworkProxy m_socksProxy;
    QList<HttpProxyStream*> m_streams;
};

#endif // HTTPPROXYSERVER_H
