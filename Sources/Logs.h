#ifndef LOGS_H
#define LOGS_H

#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>

namespace Logs {

static const int RecordsCountMax = 200; // Recent records kept in memory to be shown in log viewer

struct Record
{
    quint64 serial = 0; // Increments with every record, lets to skip records already seen
    QDateTime timestamp;
    QString message;
};

// Notifies about posted records, may emit from any thread posting messages

class Notifier : public QObject
{
    Q_OBJECT

signals:
    void recordPosted(const Logs::Record& record);
};

void init(const QString& path);

QString dirPath();
QList<Record> recentRecords();
Notifier* notifier();

}
#endif // LOGS_H
