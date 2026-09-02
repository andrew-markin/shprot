#include "PrivateKeyHighlighter.h"

PrivateKeyHighlighter::PrivateKeyHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent)
{
    m_boundersFormat.setFontWeight(QFont::Bold);
    m_boundersRegEx.setPattern(R"(^-----BEGIN.*-----|^-----END.*-----)");
}

void PrivateKeyHighlighter::highlightBlock(const QString& text)
{
    auto boundersMatch = m_boundersRegEx.match(text);

    if (boundersMatch.hasMatch())
    {
        setFormat(0, text.length(), m_boundersFormat);
        return;
    }
}
