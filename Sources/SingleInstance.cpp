#include "SingleInstance.h"

#include <QDebug>
#include <QDir>
#include <QLocalServer>
#include <QLocalSocket>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTimer>

#include <memory>

#if defined(Q_OS_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h> // AllowSetForegroundWindow, in user32, which MSVC links by default
#endif

namespace {

static const char* const ActivateMessage = "activate";
static const char* const QuitMessage = "quit";

static const int ConnectTimeout = 500; // 500ms
static const int WriteTimeout = 5000; // 5s
static const int ReadTimeout = 5000; // 5s
static const int MessageSizeMax = 64;

// The server name has a colon, which file names on Windows cannot have
QString getLockFileName(const QString& name)
{
    QString fileName = name;
    fileName.replace(QRegularExpression(R"([<>:"/\\|?*])"), "-");
    return fileName + ".lock";
}

} // namespace

SingleInstance::SingleInstance(const QString& name, const QString& lockDirectory, QObject* parent)
    : QObject(parent), m_name(name), m_lockFile(QDir(lockDirectory).filePath(getLockFileName(name))), m_server(nullptr)
{
    // Held as long as the application runs: only a lock of a process that is gone is stale
    m_lockFile.setStaleLockTime(0);
}

bool SingleInstance::start()
{
    if (!m_lockFile.tryLock(0))
    {
        sendMessage(m_name, ActivateMessage);
        return false;
    }

    // Only the holder of the lock gets here, so a server left by a crash is safe to remove
    QLocalServer::removeServer(m_name);

    m_server = new QLocalServer(this);

#if defined(Q_OS_UNIX)
    // Allow the owner only, the socket is in a directory shared by all users. Root bypasses file
    // permissions, so package scripts can still send "quit". Windows named pipes are already
    // writable by the owner, administrators and LocalSystem only.
    m_server->setSocketOptions(QLocalServer::UserAccessOption);
#endif

    connect(m_server, &QLocalServer::newConnection, this, &SingleInstance::acceptConnections);

    if (!m_server->listen(m_name))
    {
        // Without the server package scripts cannot stop the application before an upgrade
        qWarning() << "Unable to start single instance server:" << m_server->errorString();
        return false;
    }

    return true;
}

bool SingleInstance::sendMessage(const QString& name, const QByteArray& message)
{
    QLocalSocket socket;
    socket.connectToServer(name);

    if (!socket.waitForConnected(ConnectTimeout))
    {
        return false;
    }

#if defined(Q_OS_WINDOWS)
    if (message == ActivateMessage)
    {
        // Windows keeps a process in the background from taking the foreground. This one was just
        // started by the user, so it may pass that right on to the running one.
        AllowSetForegroundWindow(ASFW_ANY);
    }
#endif

    socket.write(message);
    bool written = socket.waitForBytesWritten(WriteTimeout);
    socket.disconnectFromServer();

    return written;
}

QString SingleInstance::nameForUser(const QString& userName)
{
    return QString("%1:%2").arg(PROJECT_NAME).arg(userName);
}

QString SingleInstance::defaultLockDirectory()
{
    QString directory = QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);
    return directory.isEmpty() ? QDir::tempPath() : directory;
}

void SingleInstance::acceptConnections()
{
    while (m_server->hasPendingConnections())
    {
        QLocalSocket* socket = m_server->nextPendingConnection();

        if (!socket)
        {
            return;
        }

        // The message is complete when the sender disconnects
        std::shared_ptr<QByteArray> message = std::make_shared<QByteArray>();

        auto readMessage = [socket, message]() {
            if (socket->bytesAvailable() <= 0)
            {
                return; // Nothing to read (reading a closed socket produces warnings)
            }

            message->append(socket->readAll());

            if (message->size() > MessageSizeMax)
            {
                message->clear();
                socket->abort(); // Not a message from another instance
            }
        };

        auto finishMessage = [this, socket, message]() {
            if (socket->property("handled").toBool())
            {
                return;
            }

            socket->setProperty("handled", true);
            socket->deleteLater();

            handleMessage(*message);
        };

        connect(socket, &QLocalSocket::readyRead, socket, readMessage);
        connect(socket, &QLocalSocket::disconnected, socket, [readMessage, finishMessage]() {
            readMessage();
            finishMessage();
        });

        QTimer::singleShot(ReadTimeout, socket, [socket]() {
            socket->abort(); // Sender did not disconnect in time
        });

        readMessage(); // Sender may have already sent the message and disconnected

        if (socket->state() == QLocalSocket::UnconnectedState)
        {
            finishMessage();
        }
    }
}

void SingleInstance::handleMessage(const QByteArray& message)
{
    if (message == ActivateMessage)
    {
        emit activationRequested();
    }
    else if (message == QuitMessage)
    {
        emit quitRequested();
    }
}
