#include "Logs.h"

#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QTextStream>

namespace Logs {

QString logDirPath;
QMutex logFileMutex;
QDate logFileDate;
QFile logFile;

QTextStream stdoutStream(stdout, QIODevice::WriteOnly);

const int LogFilesCountMax = 30;

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message);

void removeOldLogs()
{
    QDir logsDir(logDirPath);
    QStringList allLogFileNames = logsDir.entryList(QStringList(("*.log")), QDir::Files, QDir::Name);
    QStringList oldLogFileNames = allLogFileNames.mid(0, qMax(0, allLogFileNames.count() - LogFilesCountMax));

    for (const QString& logFileName : oldLogFileNames)
    {
        logsDir.remove(logFileName);
    }
}

void init(const QString& path)
{
    QMutexLocker locker(&logFileMutex);
    logDirPath = path;
    qInstallMessageHandler(messageHandler);
    removeOldLogs();
}

void post(const QString& message, bool save = false)
{
    QMutexLocker locker(&logFileMutex);

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDate currentDate = currentDateTime.date();

    QString normalizedMessage = message;
    normalizedMessage.replace("\r", "\\r");
    normalizedMessage.replace("\n", "\\n");
    QString timestamp = currentDateTime.toString("hh:mm:ss.zzz");
    QString record = QString("[%1] %2\r\n").arg(timestamp).arg(normalizedMessage);

    stdoutStream << record << Qt::endl << Qt::flush;

    if (!logFile.isWritable() || (currentDate != logFileDate))
    {
        logFileDate = currentDate;

        if (logFile.isOpen())
        {
            logFile.close();
        }

        QDir logDir(logDirPath);
        logDir.mkpath(".");

        QString logsFilePath = logDir.filePath(QString("%1.log").arg(logFileDate.toString("yyyy-MM-dd")));
        logFile.setFileName(logsFilePath);

        if (!logFile.open(QIODevice::Append))
        {
            return;
        }

        if (logFile.size() == 0)
        {
            logFile.write("\xEF\xBB\xBF", 3); // UTF-8 BOM
            logFile.flush();
        }

        removeOldLogs();
    }

    logFile.write(record.toUtf8());
    logFile.flush();
}

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    Q_UNUSED(context);

    switch (type)
    {
    case QtInfoMsg:
    case QtDebugMsg:
        post(message, false);
        break;
    case QtWarningMsg:
    case QtCriticalMsg:
        post(message, true);
        break;
    case QtFatalMsg:
        post(message, true);
        abort();
    }
}

}
