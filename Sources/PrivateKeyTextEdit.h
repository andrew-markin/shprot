#ifndef PRIVATEKEYTEXTEDIT_H
#define PRIVATEKEYTEXTEDIT_H

#include <QPlainTextEdit>

class QTimer;

class PrivateKeyTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit PrivateKeyTextEdit(QWidget* parent = nullptr);

public slots:
    void reveal(int delay = 1000);

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private slots:
    void conceal();

private:
    bool m_revealed = false;
    QTimer* m_concealTimer;
};

#endif // PRIVATEKEYTEXTEDIT_H
