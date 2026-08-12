#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui
{
    class PreferencesDialog;
}

class Preferences;
class QLineEdit;
class QPlainTextEdit;

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(Preferences* preferences, QWidget* parent = 0);
    ~PreferencesDialog();

public slots:
    void open();
    void accept();

protected:
    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);

private:
    void setWidgetHighlighted(QWidget* widget, bool value = true);
    void connectHighlightReset(QLineEdit* edit);
    void connectHighlightReset(QPlainTextEdit* edit);

    QStringList validate();

private slots:
    bool runTrustCheck(bool forceProbe);
    bool runTrustCheckWithForcedProbe();

    void generateSshPrivateKey();
    void importSshPrivateKey();
    void copySshPublicKey();

private:
    Ui::PreferencesDialog* m_ui;
    Preferences* m_preferences;
};

#endif // PREFERENCESDIALOG_H
