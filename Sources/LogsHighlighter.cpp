#include "LogsHighlighter.h"

#include <QDate>
#include <QLocale>

LogsHighlighter::LogsHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent)
{
    m_dateFormat.setFontWeight(QFont::Bold);
    m_timeFormat.setFontWeight(QFont::Bold);
    m_dateRegEx.setPattern(R"(^\d\d\d\d-\d\d-\d\d, .+$)");
    m_timeRegEx.setPattern(R"(^\[\d\d:\d\d:\d\d\.\d\d\d\])");
}

QString LogsHighlighter::dateSeparator(const QDate& date)
{
    return QString("%1, %2").arg(date.toString("yyyy-MM-dd"), QLocale().dayName(date.dayOfWeek()));
}

void LogsHighlighter::highlightBlock(const QString& text)
{
    auto dateMatch = m_dateRegEx.match(text);

    if (dateMatch.hasMatch())
    {
        setFormat(0, text.length(), m_dateFormat);
        return;
    }

    auto timeMatch = m_timeRegEx.match(text);

    if (timeMatch.hasMatch())
    {
        setFormat(0, timeMatch.capturedLength(), m_timeFormat);
    }
}
