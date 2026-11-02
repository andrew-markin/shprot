#include "LogsDialog.h"

#include <QAbstractTextDocumentLayout>
#include <QtMath>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QDir>
#include <QFontDatabase>
#include <QPushButton>
#include <QScrollBar>
#include <QStyle>
#include <QTextBlock>
#include <QTextEdit>
#include <QUrl>
#include <QVBoxLayout>

#include "LogsHighlighter.h"
#include "Metrics.h"

namespace {

// Log text fits a traditional terminal size at least

static const int LogTextColumns = 80;
static const int LogTextRows = 24;

} // namespace

LogsDialog::LogsDialog(QWidget* parent) : QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(QString("%1 Logs").arg(PROJECT_TITLE));

    // QTextEdit is used instead of QPlainTextEdit, since the latter ignores block indents

    m_textEdit = new QTextEdit();
    m_textEdit->setReadOnly(true);
    m_textEdit->setAcceptRichText(false);
    m_textEdit->setUndoRedoEnabled(false);
    m_textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    m_textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    m_textEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    // Timestamps are highlighted bold, so their width is measured with bold font

    QFont timestampFont = m_textEdit->font();
    timestampFont.setBold(true);
    qreal timestampWidth = QFontMetricsF(timestampFont).horizontalAdvance("[00:00:00.000] ");

    m_recordFormat.setLeftMargin(timestampWidth);
    m_recordFormat.setTextIndent(-timestampWidth);

    // Empty lines around date separators are made with margins, so they are not a part of the text

    qreal lineSpacing = QFontMetricsF(m_textEdit->font()).lineSpacing();

    m_separatorFormat.setTopMargin(lineSpacing);
    m_separatorFormat.setBottomMargin(lineSpacing);

    new LogsHighlighter(m_textEdit->document());

    QVBoxLayout* contentLayout = new QVBoxLayout();
    contentLayout->addWidget(m_textEdit, 1);

    QDialogButtonBox* buttonBox = Metrics::setupDialogLayout(this, contentLayout);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    QPushButton* openFolderButton = buttonBox->addButton("Open Logs Folder", QDialogButtonBox::ActionRole);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(openFolderButton, &QPushButton::clicked, this, &LogsDialog::openLogsFolder);
    connect(Logs::notifier(), &Logs::Notifier::recordPosted, this, &LogsDialog::appendRecord);

    // Scroll range is updated after the document layout, so following new records is done on its change

    QScrollBar* scrollBar = m_textEdit->verticalScrollBar();

    connect(scrollBar, &QScrollBar::valueChanged, this, [this, scrollBar](int value) {
        m_followingNewRecords = value == scrollBar->maximum();
    });

    connect(scrollBar, &QScrollBar::rangeChanged, this, [this, scrollBar](int minimum, int maximum) {
        Q_UNUSED(minimum);

        if (m_followingNewRecords)
        {
            scrollBar->setValue(maximum);
        }
    });

    QFontMetricsF logFontMetrics(m_textEdit->font());
    qreal logTextWidth = logFontMetrics.horizontalAdvance(QString(LogTextColumns, 'M'));
    qreal logTextHeight = logFontMetrics.lineSpacing() * LogTextRows;

    int scrollBarWidth = style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    qreal frameWidth = 2 * (m_textEdit->frameWidth() + m_textEdit->document()->documentMargin());

    m_textEdit->setMinimumSize(qCeil(logTextWidth + scrollBarWidth + frameWidth), qCeil(logTextHeight + frameWidth));
    resize(sizeHint());
}

void LogsDialog::open()
{
    if (!isVisible())
    {
        reload();
    }

    show();
    raise();
    activateWindow();
}

void LogsDialog::appendRecord(const Logs::Record& record)
{
    // Records posted while hidden are picked up on the next opening
    if (!isVisible() || (record.serial <= m_lastRecordSerial))
    {
        return;
    }

    QScrollBar* scrollBar = m_textEdit->verticalScrollBar();
    int scrollValue = scrollBar->value();

    int removedHeight = insertRecord(record);

    // Removing the oldest records does not scroll the view, so the same lines are kept in view explicitly

    if (!m_followingNewRecords)
    {
        scrollBar->setValue(qMax(0, scrollValue - removedHeight));
    }
}

void LogsDialog::openLogsFolder()
{
    QString logsDirPath = Logs::dirPath();
    QDir().mkpath(logsDirPath);
    QDesktopServices::openUrl(QUrl::fromLocalFile(logsDirPath));
}

void LogsDialog::reload()
{
    m_textEdit->clear();
    m_dayRecordCounts.clear();
    m_recordsCount = 0;
    m_lastRecordSerial = 0;
    m_followingNewRecords = true;

    for (const Logs::Record& record : Logs::recentRecords())
    {
        insertRecord(record);
    }

    QScrollBar* scrollBar = m_textEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

// Appends the record, preceded by a date separator when the day changes, and removes the oldest
// records above the limit. Returns the height of removed blocks in pixels.

int LogsDialog::insertRecord(const Logs::Record& record)
{
    m_lastRecordSerial = record.serial;

    QTextCursor cursor(m_textEdit->document());
    cursor.movePosition(QTextCursor::End);
    cursor.beginEditBlock();

    auto insertLine = [&](const QString& text, const QTextBlockFormat& format) {
        if (m_dayRecordCounts.isEmpty())
        {
            cursor.setBlockFormat(format);
        }
        else
        {
            cursor.insertBlock(format);
        }

        cursor.insertText(text);
    };

    QDate date = record.timestamp.date();

    if (m_dayRecordCounts.isEmpty() || (m_dayRecordCounts.last().first != date))
    {
        QTextBlockFormat separatorFormat = m_dayRecordCounts.isEmpty() ? topSeparatorFormat() : m_separatorFormat;
        insertLine(LogsHighlighter::dateSeparator(date), separatorFormat);
        m_dayRecordCounts.append(qMakePair(date, 0));
    }

    insertLine(QString("[%1] %2").arg(record.timestamp.toString("hh:mm:ss.zzz"), record.message), m_recordFormat);
    m_dayRecordCounts.last().second++;
    m_recordsCount++;

    qreal removedHeight = 0;

    while (m_recordsCount > Logs::RecordsCountMax)
    {
        // The first block is always a date separator, the oldest record follows it
        removedHeight += removeBlock(1);
        m_recordsCount--;

        if (--m_dayRecordCounts.first().second == 0)
        {
            removedHeight += removeBlock(0);
            m_dayRecordCounts.removeFirst();

            // The next separator becomes the top one and loses its top margin
            QTextCursor firstBlockCursor(m_textEdit->document()->firstBlock());
            firstBlockCursor.setBlockFormat(topSeparatorFormat());
            removedHeight += m_separatorFormat.topMargin();
        }
    }

    cursor.endEditBlock();
    return qRound(removedHeight);
}

QTextBlockFormat LogsDialog::topSeparatorFormat() const
{
    QTextBlockFormat format = m_separatorFormat;
    format.setTopMargin(0);
    return format;
}

// Returns the height of removed block in pixels

qreal LogsDialog::removeBlock(int number)
{
    QTextDocument* document = m_textEdit->document();
    QTextBlock block = document->findBlockByNumber(number);
    qreal height = document->documentLayout()->blockBoundingRect(block).height();

    // The paragraph separator before the block is removed with it, so the following block keeps its format

    QTextCursor cursor(block);

    if (number > 0)
    {
        cursor.movePosition(QTextCursor::PreviousCharacter);
        cursor.setPosition(block.position() + block.length() - 1, QTextCursor::KeepAnchor);
    }
    else
    {
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
    }

    cursor.removeSelectedText();

    return height;
}
