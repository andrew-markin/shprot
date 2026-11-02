#ifndef LOGSHIGHLIGHTER_H
#define LOGSHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class LogsHighlighter : public QSyntaxHighlighter
{
public:
    explicit LogsHighlighter(QTextDocument* parent = nullptr);

    static QString dateSeparator(const QDate& date);

protected:
    void highlightBlock(const QString &text) override;

private:
    QTextCharFormat m_dateFormat;
    QTextCharFormat m_timeFormat;
    QRegularExpression m_dateRegEx;
    QRegularExpression m_timeRegEx;
};

#endif // LOGSHIGHLIGHTER_H
