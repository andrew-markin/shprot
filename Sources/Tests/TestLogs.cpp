#include <QFile>
#include <QTemporaryDir>
#include <QTest>

#include "Logs.h"

// These records are lost on a parsing error, so the log viewer shows an incomplete history
class TestLogs : public QObject
{
    Q_OBJECT

private slots:
    void init();

    void recordsAreParsed();
    void nonRecordLinesAreSkipped();
    void tailOfALargeFileIsRead();

private:
    void writeFile(const QByteArray& content) const;
    QList<Logs::Record> readRecords() const;

private:
    QTemporaryDir* m_temporaryDir = nullptr;
};

void TestLogs::init()
{
    m_temporaryDir = new QTemporaryDir();
    QVERIFY(m_temporaryDir->isValid());
}

void TestLogs::writeFile(const QByteArray& content) const
{
    QFile file(m_temporaryDir->filePath("2026-11-18.log"));
    QVERIFY(file.open(QIODevice::WriteOnly));
    QCOMPARE(file.write(content), qint64(content.size()));
}

QList<Logs::Record> TestLogs::readRecords() const
{
    QDate date = QDate(2026, 11, 18);
    return Logs::readLogFileRecords(m_temporaryDir->filePath("2026-11-18.log"), date);
}

void TestLogs::recordsAreParsed()
{
    writeFile("\xEF\xBB\xBF" // The file starts with a BOM
              "[12:34:56.789] First message\r\n"
              "[01:02:03.004] Second message\r\n");

    QList<Logs::Record> records = readRecords();

    QCOMPARE(records.size(), 2);
    QCOMPARE(records.first().timestamp, QDateTime(QDate(2026, 11, 18), QTime(12, 34, 56, 789)));
    QCOMPARE(records.first().message, QString("First message"));
    QCOMPARE(records.last().message, QString("Second message"));
}

void TestLogs::nonRecordLinesAreSkipped()
{
    writeFile("not a record\r\n"
              "\r\n"
              "[12:00:00.000] A record among the garbage\r\n"
              "[not a time] Still garbage\r\n"
              "[12:30:00.000] The last record\r\n"
              "[12:40:00.000] Missing the final line break");

    QList<Logs::Record> records = readRecords();

    // The record without the final line break at the end of the file is parsed as well
    QCOMPARE(records.size(), 3);
    QCOMPARE(records.first().message, QString("A record among the garbage"));
    QCOMPARE(records[1].message, QString("The last record"));
    QCOMPARE(records.last().message, QString("Missing the final line break"));
}

void TestLogs::tailOfALargeFileIsRead()
{
    QByteArray tail;

    for (int i = 0; i < 1000; i++)
    {
        tail += QString("[12:%1:%2.%3] Record %4\r\n").arg(i / 60, 2, 10, QChar('0'))
                    .arg(i % 60, 2, 10, QChar('0')).arg(i % 1000, 3, 10, QChar('0')).arg(i).toUtf8();
    }

    QByteArray filler(300 * 1024, 'x'); // Older records beyond the read tail
    writeFile(filler + "cut-in-the-middle" + tail);

    QList<Logs::Record> records = readRecords();

    // The tail window may start inside a record, only that one line is lost
    QVERIFY(records.size() >= 999);
    QCOMPARE(records.last().message, QString("Record 999"));
}

QTEST_GUILESS_MAIN(TestLogs)

#include "TestLogs.moc"
