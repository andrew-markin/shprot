#ifndef PRIVATEKEYHIGHLIGHTER_H
#define PRIVATEKEYHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class PrivateKeyHighlighter : public QSyntaxHighlighter
{
public:
    explicit PrivateKeyHighlighter(QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    QTextCharFormat m_boundersFormat;
    QRegularExpression m_boundersRegEx;
};

#endif // PRIVATEKEYHIGHLIGHTER_H
