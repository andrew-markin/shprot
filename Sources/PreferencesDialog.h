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
    void reject();
    void reset();

protected:
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;

private:
    void setWidgetHighlighted(QWidget* widget, bool value = true);
    void connectHighlightReset(QLineEdit* edit);
    void connectHighlightReset(QPlainTextEdit* edit);

    bool hasChanges() const;

    QStringList validateSshDestinationAndPort();
    QStringList validate();

private slots:
    bool runTrustCheck(bool forceProbe);
    void handleTrustCheckButtonClick();

    void generateSshPrivateKey();
    void importSshPrivateKey();
    void copySshPublicKey();

private:
    Ui::PreferencesDialog* m_ui;
    Preferences* m_preferences;
    QPalette m_highlightPalette;
};

#endif // PREFERENCESDIALOG_H
