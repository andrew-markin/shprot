#include "PrivateKeyTextEdit.h"

#include <QApplication>
#include <QMenu>
#include <QPainter>
#include <QStyle>
#include <QTextBlock>
#include <QTimer>
#include <QtMath>

#include "PrivateKeyHighlighter.h"

PrivateKeyTextEdit::PrivateKeyTextEdit(QWidget* parent) : QPlainTextEdit(parent)
{
    QFont defaultFont = QApplication::font();
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    fixedFont.setPointSizeF(defaultFont.pointSizeF());
    setFont(fixedFont);

    // Fits the longest line of OpenSSH private key (70 characters) without horizontal scrolling,
    // even when vertical scrollbar is shown for a long key
    QFontMetrics fixedFontMetrics(fixedFont);
    setMinimumWidth(fixedFontMetrics.horizontalAdvance(QByteArray(70, 'W')) +
                    qCeil(document()->documentMargin() * 2) +
                    frameWidth() * 2 +
                    style()->pixelMetric(QStyle::PM_ScrollBarExtent, nullptr, this));
    setMinimumHeight(fixedFontMetrics.lineSpacing() * 10);

    setLineWrapMode(QPlainTextEdit::NoWrap);

    new PrivateKeyHighlighter(document());

    m_concealTimer = new QTimer(this);
    m_concealTimer->setSingleShot(true);
    connect(m_concealTimer, &QTimer::timeout, this, &PrivateKeyTextEdit::conceal);
}

void PrivateKeyTextEdit::reveal(int delay)
{
    if (!m_revealed)
    {
        m_revealed = true;
        viewport()->update();
    }

    m_concealTimer->start(delay);
}

void PrivateKeyTextEdit::contextMenuEvent(QContextMenuEvent* event)
{
    m_revealed = true;
    QMenu* menu = createStandardContextMenu();
    menu->exec(QCursor::pos());
    menu->deleteLater();
    m_revealed = false;
    setFocus();
}

void PrivateKeyTextEdit::focusInEvent(QFocusEvent* event)
{
    QPlainTextEdit::focusInEvent(event);
    viewport()->update();
}

void PrivateKeyTextEdit::focusOutEvent(QFocusEvent* event)
{
    QPlainTextEdit::focusOutEvent(event);
    viewport()->update();
}

void PrivateKeyTextEdit::paintEvent(QPaintEvent* event)
{
    if (hasFocus() || m_revealed)
    {
        QPlainTextEdit::paintEvent(event);
        return;
    }

    QWidget* viewport = QPlainTextEdit::viewport();
    QPalette palette = QPlainTextEdit::palette();

    QPainter painter(viewport);
    painter.fillRect(viewport->rect(), palette.color(QPalette::Base));

    painter.setPen(palette.color(QPalette::Text));
    painter.setFont(viewport->font());
    painter.setOpacity(0.75);

    qreal documentMargin = document()->documentMargin();
    QFontMetrics fontMetrics = viewport->fontMetrics();
    QPointF drawOffset(documentMargin, documentMargin + fontMetrics.ascent());

    static const QChar MaskChar(0x2022);

    QTextBlock block = firstVisibleBlock();

    while (block.isValid())
    {
        if (block.isVisible())
        {
            QString maskedText = block.text();

            for (QChar& ch : maskedText)
            {
                if ((ch != ' ') && (ch != '\t'))
                {
                    ch = MaskChar;
                }
            }

            painter.drawText(blockBoundingGeometry(block).topLeft() + drawOffset, maskedText);
        }

        block = block.next();
    }
}

void PrivateKeyTextEdit::conceal()
{
    if (m_revealed)
    {
        m_revealed = false;
        viewport()->update();
    }
}
