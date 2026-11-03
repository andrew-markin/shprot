#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

class Preferences;
class PrivateKeyTextEdit;
class QCheckBox;
class QLineEdit;
class QPlainTextEdit;
class QToolButton;

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(Preferences* preferences, QWidget* parent = nullptr);

public slots:
    void open();
    void accept();
    void reject();
    void reset();

protected:
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void clearChildrenFocus();
    void setWidgetHighlighted(QWidget* widget, bool value = true);
    void connectHighlightReset(QLineEdit* edit);
    void connectHighlightReset(QPlainTextEdit* edit);

    bool hasChanges() const;

    bool confirmExposure();

    QStringList validateSshDestinationAndPort();
    QStringList validate();

    void copySshPublicKey(bool asInstallCommand);

private slots:
    bool runTrustCheck(bool forceProbe);
    void handleTrustCheckButtonClick();

    void generateSshPrivateKey();
    void importSshPrivateKey();

private:
    Preferences* m_preferences;
    QPalette m_highlightPalette;

    QCheckBox* m_sshProxyTunnelCheckBox;
    QLineEdit* m_sshDestinationEdit;
    QLineEdit* m_sshPortEdit;
    QToolButton* m_trustCheckButton;
    QToolButton* m_copySshPublicKeyButton;
    PrivateKeyTextEdit* m_sshPrivateKeyEdit;
    QLineEdit* m_localSocks5ProxyHostEdit;
    QLineEdit* m_localSocks5ProxyPortEdit;
    QCheckBox* m_localHttpProxyCheckBox;
    QLineEdit* m_localHttpProxyHostEdit;
    QLineEdit* m_localHttpProxyPortEdit;
};

#endif // PREFERENCESDIALOG_H
