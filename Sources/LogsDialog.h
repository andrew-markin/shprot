#ifndef LOGSDIALOG_H
#define LOGSDIALOG_H

#include <QDate>
#include <QDialog>
#include <QList>
#include <QPair>
#include <QTextBlockFormat>

#include "Logs.h"

class QTextEdit;

class LogsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogsDialog(QWidget* parent = nullptr);

public slots:
    void open();

private slots:
    void appendRecord(const Logs::Record& record);
    void openLogsFolder();

private:
    void reload();
    int insertRecord(const Logs::Record& record);
    qreal removeBlock(int number);
    QTextBlockFormat topSeparatorFormat() const;

private:
    QTextEdit* m_textEdit;
    QTextBlockFormat m_recordFormat; // Wrapped lines of a record are indented by the timestamp width
    QTextBlockFormat m_separatorFormat; // Date separator is surrounded by empty lines, except the top one
    QList<QPair<QDate, int>> m_dayRecordCounts; // Shown records grouped by day, each group follows a date separator
    int m_recordsCount = 0;
    bool m_followingNewRecords = true; // Scrolled to the bottom, so new records are kept in view
    quint64 m_lastRecordSerial = 0;
};

#endif // LOGSDIALOG_H
