#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class QPlainTextEdit;
class QTabWidget;

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget* parent = nullptr);

public slots:
    void open();

private:
    QPlainTextEdit* addLicenseTab(const QString& title, const QString& text);

private:
    QTabWidget* m_tabWidget;
};

#endif // ABOUTDIALOG_H
