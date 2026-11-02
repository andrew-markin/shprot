#include "Logs.h"

#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMutex>
#include <QRegularExpression>
#include <QTextStream>

namespace Logs {

QString logDirPath;
QMutex logFileMutex;
QDate logFileDate;
QFile logFile;

QList<Record> records;
quint64 lastRecordSerial = 0;

QTextStream stdoutStream(stdout, QIODevice::WriteOnly);

const int LogFilesAgeMax = 30; // Days
const qint64 LogFileTailSizeMax = 256 * 1024; // Only the tail of a log file is read to restore recent records

const char* LogFileDateFormat = "yyyy-MM-dd";

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message);

QStringList logFileNames()
{
    QDir logsDir(logDirPath);
    return logsDir.entryList(QStringList(("*.log")), QDir::Files, QDir::Name);
}

QDate logFileNameDate(const QString& logFileName)
{
    return QDate::fromString(QFileInfo(logFileName).completeBaseName(), LogFileDateFormat);
}

void removeOldLogs()
{
    QDir logsDir(logDirPath);
    QDate currentDate = QDate::currentDate();

    for (const QString& logFileName : logFileNames())
    {
        QDate logFileDate = logFileNameDate(logFileName);

        if (logFileDate.isValid() && (logFileDate.daysTo(currentDate) > LogFilesAgeMax))
        {
            logsDir.remove(logFileName);
        }
    }
}

QList<Record> readLogFileRecords(const QString& logFilePath, const QDate& date)
{
    QList<Record> fileRecords;
    QFile file(logFilePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        return fileRecords;
    }

    bool truncated = file.size() > LogFileTailSizeMax;

    if (truncated)
    {
        file.seek(file.size() - LogFileTailSizeMax);
    }

    QStringList lines = QString::fromUtf8(file.readAll()).split("\r\n", Qt::SkipEmptyParts);

    if (truncated && !lines.isEmpty())
    {
        lines.removeFirst(); // The first line is likely cut in the middle
    }

    static const QRegularExpression RecordRegEx(R"(^(?:\x{FEFF})?\[(\d\d:\d\d:\d\d\.\d\d\d)\] (.*)$)");

    for (const QString& line : lines)
    {
        QRegularExpressionMatch match = RecordRegEx.match(line);

        if (!match.hasMatch())
        {
            continue;
        }

        QTime time = QTime::fromString(match.captured(1), "hh:mm:ss.zzz");

        if (!time.isValid())
        {
            continue;
        }

        Record record;
        record.timestamp = QDateTime(date, time);
        record.message = match.captured(2);
        fileRecords.append(record);
    }

    return fileRecords;
}

// Restores recent records from the latest log files, so log viewer shows them after restart

void restoreRecentRecords()
{
    QDir logsDir(logDirPath);
    QStringList allLogFileNames = logFileNames();

    for (auto it = allLogFileNames.crbegin(); it != allLogFileNames.crend(); ++it)
    {
        QDate date = logFileNameDate(*it);

        if (!date.isValid())
        {
            continue;
        }

        QList<Record> fileRecords = readLogFileRecords(logsDir.filePath(*it), date);
        records = fileRecords.mid(qMax(0, fileRecords.count() - (RecordsCountMax - records.count()))) + records;

        if (records.count() >= RecordsCountMax)
        {
            break;
        }
    }

    for (Record& record : records)
    {
        record.serial = ++lastRecordSerial;
    }
}

void init(const QString& path)
{
    QMutexLocker locker(&logFileMutex);
    logDirPath = path;
    qInstallMessageHandler(messageHandler);
    removeOldLogs();
    restoreRecentRecords();
}

QString dirPath()
{
    QMutexLocker locker(&logFileMutex);
    return logDirPath;
}

QList<Record> recentRecords()
{
    QMutexLocker locker(&logFileMutex);
    return records;
}

Notifier* notifier()
{
    static Notifier instance;
    return &instance;
}

void writeToLogFile(const QString& text, const QDate& currentDate)
{
    if (!logFile.isWritable() || (currentDate != logFileDate))
    {
        logFileDate = currentDate;

        if (logFile.isOpen())
        {
            logFile.close();
        }

        QDir logDir(logDirPath);
        logDir.mkpath(".");

        QString logsFilePath = logDir.filePath(QString("%1.log").arg(logFileDate.toString(LogFileDateFormat)));
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

    logFile.write(text.toUtf8() + "\r\n");
    logFile.flush();
}

void post(const QString& message, bool save = false)
{
    Record record;

    {
        QMutexLocker locker(&logFileMutex);

        QDateTime currentDateTime = QDateTime::currentDateTime();

        QString normalizedMessage = message;
        normalizedMessage.replace("\r", "\\r");
        normalizedMessage.replace("\n", "\\n");
        QString timestamp = currentDateTime.toString("hh:mm:ss.zzz");
        QString text = QString("[%1] %2").arg(timestamp).arg(normalizedMessage);

        stdoutStream << text << Qt::endl; // Qt::endl flushes the stream

        if (save)
        {
            writeToLogFile(text, currentDateTime.date()); // Debug and info messages are not saved to the log file
        }

        record.serial = ++lastRecordSerial;
        record.timestamp = currentDateTime;
        record.message = normalizedMessage;

        records.append(record);

        if (records.count() > RecordsCountMax)
        {
            records.removeFirst();
        }
    }

    // Emitted without the lock held, since receivers may post messages too
    emit notifier()->recordPosted(record);
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
