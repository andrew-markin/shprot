#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QLockFile>
#include <QObject>

class QLocalServer;

// One running application per user. The lock file decides who runs, even for starts at the same
// moment, the local server only carries messages: "activate" from a second start, "quit" from
// package scripts before an upgrade or removal.
class SingleInstance : public QObject
{
    Q_OBJECT

public:
    // The name tells users and applications apart, the directory holds the lock file
    SingleInstance(const QString& name, const QString& lockDirectory, QObject* parent = nullptr);

    // True for the only instance, false if another one runs (it is asked to activate) or listening failed
    bool start();

    // Delivers a message to the running instance, never touches the lock, so it works for any user
    // allowed to connect (root for package scripts)
    static bool sendMessage(const QString& name, const QByteArray& message);

    static QString nameForUser(const QString& userName);
    static QString defaultLockDirectory();

signals:
    void activationRequested();
    void quitRequested();

private slots:
    void acceptConnections();

private:
    void handleMessage(const QByteArray& message);

private:
    QString m_name;
    QLockFile m_lockFile;
    QLocalServer* m_server;
};

#endif // SINGLEINSTANCE_H
