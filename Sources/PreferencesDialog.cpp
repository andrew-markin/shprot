#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontMetrics>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QStringList>
#include <QTimer>

#include "Preferences.h"
#include "Utilities.h"
#include "Websites.h"

namespace {

QColor blendColors(const QColor& background, const QColor& foreground, int alpha = 128)
{
    double a = qBound(0, alpha, 255) / 255.0;

    if (a == 0.0)
    {
        return background;
    }

    if (a == 1.0)
    {
        return foreground;
    }

    int r = static_cast<int>(foreground.red() * a + background.red() * (1.0 - a));
    int g = static_cast<int>(foreground.green() * a + background.green() * (1.0 - a));
    int b = static_cast<int>(foreground.blue() * a + background.blue() * (1.0 - a));

    return QColor(r, g, b, 255);
}

bool portStringIsAcceptable(const QString& portString, bool system = false)
{
    if (portString.isEmpty())
    {
        return true;
    }

    bool numberIsOk = false;
    int portNumber = portString.toInt(&numberIsOk);
    return numberIsOk && (system || (portNumber >= 1024)) && (portNumber <= 65535);
}

} // namespace

PreferencesDialog::PreferencesDialog(Preferences* preferences, QWidget* parent)
    : QDialog(parent), m_ui(new Ui::PreferencesDialog), m_preferences(preferences)
{
    m_ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowFullscreenButtonHint);
    setWindowTitle(QString("%1 Preferences").arg(PROJECT_TITLE));

    if (m_preferences->hasValue("preferencesDialogGeometry"))
    {
        restoreGeometry(QByteArray::fromBase64(m_preferences->value("preferencesDialogGeometry").toByteArray()));
    }

    // Prepare highlight palette

    QPalette sourcePalette = palette();
    m_highlightPalette = sourcePalette;

    static const QColor HilightColor("#ff5722");
    static const int HighlightBlend = 30;

    QColor baseActiveColor = sourcePalette.color(QPalette::Active, QPalette::Base);
    QColor baseActiveHighlightedColor = blendColors(baseActiveColor, HilightColor, HighlightBlend);
    m_highlightPalette.setColor(QPalette::Active, QPalette::Base, baseActiveHighlightedColor);

    QColor baseInactiveColor = sourcePalette.color(QPalette::Inactive, QPalette::Base);
    QColor baseInactiveHighlightedColor = blendColors(baseInactiveColor, HilightColor, HighlightBlend);
    m_highlightPalette.setColor(QPalette::Inactive, QPalette::Base, baseInactiveHighlightedColor);

    QColor textActiveColor = sourcePalette.color(QPalette::Active, QPalette::Text);
    QColor textActiveHighlightedColor = blendColors(textActiveColor, HilightColor, HighlightBlend);
    m_highlightPalette.setColor(QPalette::Active, QPalette::Text, textActiveHighlightedColor);

    QColor textInactiveColor = sourcePalette.color(QPalette::Inactive, QPalette::Text);
    QColor textInactiveHighlightedColor = blendColors(textInactiveColor, HilightColor, HighlightBlend);
    m_highlightPalette.setColor(QPalette::Inactive, QPalette::Text, textInactiveHighlightedColor);

    // Setup controls

    connect(m_ui->sshProxyTunnelCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        m_ui->sshProxyTunnelWidget->setEnabled(checked);
    });

    static const QRegularExpression SshDestinationRegEx(R"(^[a-zA-Z0-9._-]+\@[a-zA-Z0-9.-]+$)");
    QRegularExpressionValidator* sshDestinationValidator = new QRegularExpressionValidator(SshDestinationRegEx, this);
    m_ui->sshDestinationEdit->setValidator(sshDestinationValidator);
    connectHighlightReset(m_ui->sshDestinationEdit);

    static const QRegularExpression PortRegEx(R"(^([0-9]{0,5})$)");
    QRegularExpressionValidator* portValidator = new QRegularExpressionValidator(PortRegEx, this);

    m_ui->sshPortEdit->setValidator(portValidator);
    connectHighlightReset(m_ui->sshPortEdit);

    QFont defaultFont = QApplication::font();
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    fixedFont.setPointSizeF(defaultFont.pointSizeF());
    m_ui->sshPrivateKeyEdit->setFont(fixedFont);

    QFontMetrics fixedFontMetrics(fixedFont);
    m_ui->sshPrivateKeyEdit->setMinimumWidth(fixedFontMetrics.horizontalAdvance(QByteArray(75, 'W')));
    m_ui->sshPrivateKeyEdit->setMinimumHeight(fixedFontMetrics.lineSpacing() * 10);

    connectHighlightReset(m_ui->sshPrivateKeyEdit);

    static const QRegularExpression HostRegEx(R"(^(\[[0-9a-fA-F:]+\]|[a-zA-Z0-9.-]*)$)");
    QRegularExpressionValidator* hostValidator = new QRegularExpressionValidator(HostRegEx, this);

    m_ui->localSocks5ProxyHostEdit->setValidator(hostValidator);
    connectHighlightReset(m_ui->localSocks5ProxyHostEdit);

    m_ui->localSocks5ProxyPortEdit->setValidator(portValidator);
    connectHighlightReset(m_ui->localSocks5ProxyPortEdit);

    connect(m_ui->trustCheckButton, &QToolButton::clicked, this, &PreferencesDialog::handleTrustCheckButtonClick);
    connect(m_ui->generateSshPrivateKeyButton, &QToolButton::clicked, this, &PreferencesDialog::generateSshPrivateKey);
    connect(m_ui->importSshPrivateKeyButton, &QToolButton::clicked, this, &PreferencesDialog::importSshPrivateKey);
    connect(m_ui->copySshPublicKeyButton, &QToolButton::clicked, this, &PreferencesDialog::copySshPublicKey);

    connect(m_ui->localHttpProxyCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        m_ui->localHttpProxyAddresWidget->setEnabled(checked);
    });

    m_ui->localHttpProxyHostEdit->setValidator(hostValidator);
    connectHighlightReset(m_ui->localHttpProxyHostEdit);

    m_ui->localHttpProxyPortEdit->setValidator(portValidator);
    connectHighlightReset(m_ui->localHttpProxyPortEdit);

    QPushButton* resetButton = m_ui->buttonBox->button(QDialogButtonBox::Reset);
    connect(resetButton, &QPushButton::clicked, this, &PreferencesDialog::reset);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &PreferencesDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &PreferencesDialog::reject);
}

PreferencesDialog::~PreferencesDialog()
{
    delete m_ui;
}

void PreferencesDialog::open()
{
    if (isVisible())
    {
        activateWindow();
        raise();
        return;
    }

    reset();

    QDialog::open();
}

void PreferencesDialog::accept()
{
    QStringList errors = validate();

    if (!errors.isEmpty())
    {
        QStringList errorLines;
        int errorNumber = 1;

        for (const QString& error : errors)
        {
            errorLines.append(QString("<nobr>%1) %2</nobr>").arg(errorNumber++).arg(error));
        }

        QMessageBox messageBox(this);
        messageBox.setWindowTitle("Preferences Error");
        messageBox.setTextFormat(Qt::RichText);
        messageBox.setText(QString("One or more fields are invalid:<br><br>%1")
                           .arg(errorLines.join("<br>")));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();

        return;
    }

    if (m_ui->sshProxyTunnelCheckBox->isChecked() && !runTrustCheck(false))
    {
        return;
    }

    QVariantMap sshProxyTunnel({{"enabled", m_ui->sshProxyTunnelCheckBox->isChecked()},
                                {"sshDestination", m_ui->sshDestinationEdit->text()},
                                {"sshPort", m_ui->sshPortEdit->text()},
                                {"sshPrivateKey", m_ui->sshPrivateKeyEdit->toPlainText()},
                                {"localSocks5ProxyHost", m_ui->localSocks5ProxyHostEdit->text()},
                                {"localSocks5ProxyPort", m_ui->localSocks5ProxyPortEdit->text()},
                                {"localHttpProxyEnabled", m_ui->localHttpProxyCheckBox->isChecked()},
                                {"localHttpProxyHost", m_ui->localHttpProxyHostEdit->text()},
                                {"localHttpProxyPort", m_ui->localHttpProxyPortEdit->text()}});

    m_preferences->setValue("sshProxyTunnel", sshProxyTunnel);

    QDialog::accept();
}

void PreferencesDialog::reset()
{
    QVariantMap sshProxyTunnel = m_preferences->value("sshProxyTunnel").toMap();

    m_ui->sshProxyTunnelCheckBox->setChecked(sshProxyTunnel.value("enabled", false).toBool());
    m_ui->sshDestinationEdit->setText(sshProxyTunnel.value("sshDestination").toString());
    m_ui->sshPortEdit->setText(sshProxyTunnel.value("sshPort").toString());
    m_ui->sshPrivateKeyEdit->setPlainText(sshProxyTunnel.value("sshPrivateKey").toString());
    m_ui->localSocks5ProxyHostEdit->setText(sshProxyTunnel.value("localSocks5ProxyHost").toString());
    m_ui->localSocks5ProxyPortEdit->setText(sshProxyTunnel.value("localSocks5ProxyPort").toString());
    m_ui->localHttpProxyCheckBox->setChecked(sshProxyTunnel.value("localHttpProxyEnabled", false).toBool());
    m_ui->localHttpProxyHostEdit->setText(sshProxyTunnel.value("localHttpProxyHost").toString());
    m_ui->localHttpProxyPortEdit->setText(sshProxyTunnel.value("localHttpProxyPort").toString());

    validate();
}

void PreferencesDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    activateWindow();
}

void PreferencesDialog::hideEvent(QHideEvent* event)
{
    m_preferences->setValue("preferencesDialogGeometry", saveGeometry().toBase64());
    QDialog::hideEvent(event);
}
}

void PreferencesDialog::setWidgetHighlighted(QWidget* widget, bool value)
{
    if (widget->property("highlighted").toBool() != value)
    {
        widget->setPalette(value ? m_highlightPalette : palette());
        widget->setProperty("highlighted", value);
    }
}

void PreferencesDialog::connectHighlightReset(QLineEdit* edit)
{
    connect(edit, &QLineEdit::textChanged, this, [this, edit]() {
        setWidgetHighlighted(edit, false);
    });
}

void PreferencesDialog::connectHighlightReset(QPlainTextEdit* edit)
{
    connect(edit, &QPlainTextEdit::textChanged, this, [this, edit]() {
        setWidgetHighlighted(edit, false);
    });
}

QStringList PreferencesDialog::validateSshDestinationAndPort()
{
    QStringList errors;

    setWidgetHighlighted(m_ui->sshDestinationEdit, false);
    setWidgetHighlighted(m_ui->sshPortEdit, false);

    // SSH Destination

    if (!m_ui->sshDestinationEdit->hasAcceptableInput())
    {
        errors.append("Invalid SSH Destination");
        setWidgetHighlighted(m_ui->sshDestinationEdit, true);
    }

    // SSH Port

    if (!m_ui->sshPortEdit->hasAcceptableInput() || !portStringIsAcceptable(m_ui->sshPortEdit->text(), true))
    {
        errors.append("Invalid SSH Port");
        setWidgetHighlighted(m_ui->sshPortEdit, true);
    }

    return errors;
}

QStringList PreferencesDialog::validate()
{
    QStringList errors;

    setWidgetHighlighted(m_ui->sshDestinationEdit, false);
    setWidgetHighlighted(m_ui->sshPortEdit, false);
    setWidgetHighlighted(m_ui->sshPrivateKeyEdit, false);
    setWidgetHighlighted(m_ui->localSocks5ProxyHostEdit, false);
    setWidgetHighlighted(m_ui->localSocks5ProxyPortEdit, false);
    setWidgetHighlighted(m_ui->localHttpProxyHostEdit, false);
    setWidgetHighlighted(m_ui->localHttpProxyPortEdit, false);

    if (!m_ui->sshProxyTunnelCheckBox->isChecked())
    {
        return errors;
    }

    errors = validateSshDestinationAndPort();

    // SSH Private Key

    if (!Utilities::sshPrivateKeyLooksValid(m_ui->sshPrivateKeyEdit->toPlainText()))
    {
        errors.append("Invalid SSH Private Key");
        setWidgetHighlighted(m_ui->sshPrivateKeyEdit, true);
    }

    // Local SOCKS5 Proxy Host

    if (!m_ui->localSocks5ProxyHostEdit->hasAcceptableInput())
    {
        errors.append("Invalid Local SOCKS5 Proxy Host");
        setWidgetHighlighted(m_ui->localSocks5ProxyHostEdit, true);
    }

    // Local SOCKS5 Proxy Port

    if (!m_ui->localSocks5ProxyPortEdit->hasAcceptableInput() ||
        !portStringIsAcceptable(m_ui->localSocks5ProxyPortEdit->text()))
    {
        errors.append("Invalid Local SOCKS5 Proxy Port");
        setWidgetHighlighted(m_ui->localSocks5ProxyPortEdit, true);
    }

    // Local HTTP Proxy

    if (m_ui->localHttpProxyCheckBox->isChecked())
    {
        // Local HTTP Proxy Host

        if (!m_ui->localHttpProxyHostEdit->hasAcceptableInput())
        {
            errors.append("Invalid Local HTTP Proxy Host");
            setWidgetHighlighted(m_ui->localHttpProxyHostEdit, true);
        }

        // Local HTTP Proxy Port

        if (!m_ui->localHttpProxyPortEdit->hasAcceptableInput() ||
            !portStringIsAcceptable(m_ui->localHttpProxyPortEdit->text()))
        {
            errors.append("Invalid Local HTTP Proxy Port");
            setWidgetHighlighted(m_ui->localHttpProxyPortEdit, true);
        }
    }

    return errors;
}

bool PreferencesDialog::runTrustCheck(bool forceProbe)
{
    QStringList errors = validateSshDestinationAndPort();

    if (!errors.isEmpty())
    {
        QMessageBox::warning(this, "Trust Check Error", "Invalid SSH Destination or Port", QMessageBox::Ok);
        return false;
    }

    QString address = Utilities::getAddressFromSshDestination(m_ui->sshDestinationEdit->text());

    QString knownHostRecord = Utilities::getKnownHostRecord(address);
    bool hostIsKnown = !knownHostRecord.isEmpty();

    if (hostIsKnown && !forceProbe)
    {
        return true;
    }

    quint16 port = Utilities::parsePort(m_ui->sshPortEdit->text(), 22);
    QString probeHostRecord = Utilities::probeActualHostRecord(address, port);

    if (probeHostRecord.isEmpty())
    {
        QMessageBox::warning(this, "Trust Check Error", "Host probe failed", QMessageBox::Ok);
        return false;
    }

    auto [probeKeyType, probeKeyFingerprint] = Utilities::getKeyTypeAndFingerprint(probeHostRecord);

    if (!hostIsKnown)
    {
        QMessageBox messageBox(this);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setWindowTitle("Unknown Server Identity");
        messageBox.setTextFormat(Qt::RichText);
        messageBox.setText(
                    QString("The authenticity of host <b>%1</b> can't be established.<br><br>"
                            "%2 key fingerprint is:<br><nobr>SHA256:%3.</nobr>")
                    .arg(address)
                    .arg(Utilities::getKeyTypeLabel(probeKeyType))
                    .arg(probeKeyFingerprint));
        messageBox.setInformativeText("Are you sure you want to continue connecting and trust this server?");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::No);
        messageBox.setEscapeButton(QMessageBox::No);

        if (messageBox.exec() == QMessageBox::Yes)
        {
            Utilities::setKnownHostRecord(address, probeHostRecord);
            return true;
        }

        return false;
    }

    auto [knownKeyType, knowKeyFingerprint] = Utilities::getKeyTypeAndFingerprint(knownHostRecord);

    if ((knownKeyType != probeKeyType) || (knowKeyFingerprint != probeKeyFingerprint))
    {
        QMessageBox messageBox(this);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setWindowTitle("Remote Host Identification Has Changed");
        messageBox.setTextFormat(Qt::RichText);
        messageBox.setText(
                    QString("The server key fingerprint for <b>%1</b> does not match the previously saved value. "
                            "This could mean someone is intercepting your connection (Man-in-the-Middle attack) "
                            "or the server administrator just reinstalled the OS.<br><br>"
                            "Saved %2 key fingerprint is:<br><nobr>SHA256:%3.</nobr><br><br>"
                            "Received %4 key fingerprint is:<br><nobr>SHA256:%5.</nobr>")
                    .arg(address)
                    .arg(Utilities::getKeyTypeLabel(knownKeyType))
                    .arg(knowKeyFingerprint)
                    .arg(Utilities::getKeyTypeLabel(probeKeyType))
                    .arg(probeKeyFingerprint));
        messageBox.setInformativeText("If you are certain this change is legitimate, you can update the trusted key.");
        messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        QAbstractButton* saveButton = messageBox.button(QMessageBox::Save);
        saveButton->setText("Update Key and Trust");
        saveButton->setMinimumWidth(140);
        messageBox.setDefaultButton(QMessageBox::Cancel);
        messageBox.setEscapeButton(QMessageBox::Cancel);

        if (messageBox.exec() == QMessageBox::Save)
        {
            Utilities::setKnownHostRecord(address, probeHostRecord);
            return true;
        }

        return false;
    }

    QMessageBox::information(this, "Trust Check Succeeded", "Server identity verified successfully.", QMessageBox::Ok);
    return true;
}

void PreferencesDialog::handleTrustCheckButtonClick()
{
    QToolButton* button = m_ui->trustCheckButton;
    QString buttonSavedText = button->text();
    button->setMinimumWidth(button->width());
    button->setText("Checking Trust");
    button->setEnabled(false);

    QTimer::singleShot(50, this, [this, button, buttonSavedText]() {
        runTrustCheck(true);
        button->setText(buttonSavedText);
        button->setMinimumWidth(0);
        button->setEnabled(true);
    });
}

void PreferencesDialog::generateSshPrivateKey()
{
    m_ui->sshPrivateKeyEdit->setPlainText(Utilities::generateSshPrivateKey());
}

void PreferencesDialog::importSshPrivateKey()
{
    QFileDialog fileDialog(this);

    fileDialog.setWindowTitle("Open SSH Private Key");
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    fileDialog.setDirectory(m_preferences->value("sshPrivateKeyImportDirectory", QDir::homePath()).toString());

    if (fileDialog.exec() != QFileDialog::Accepted)
    {
        return;
    }

    m_preferences->setValue("sshPrivateKeyImportDirectory", fileDialog.directory().canonicalPath());

    QFile selectedFile(fileDialog.selectedFiles().first());

    if ((selectedFile.size() <= 32768) && selectedFile.open(QIODevice::ReadOnly))
    {
        QString privateKey = QString::fromUtf8(selectedFile.readAll());

        if (Utilities::sshPrivateKeyLooksValid(privateKey))
        {
            m_ui->sshPrivateKeyEdit->setPlainText(privateKey);
            return;
        }
    }

    QMessageBox messageBox;
    messageBox.setWindowTitle("SSH Private Key Import Error");
    messageBox.setText("Invalid SSH Private Key file");
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.exec();
}

void PreferencesDialog::copySshPublicKey()
{
    QString privateKey = m_ui->sshPrivateKeyEdit->toPlainText();
    QString publicKey = Utilities::getSshPublicKey(privateKey);
    QGuiApplication::clipboard()->setText(publicKey);

    QToolButton* button = m_ui->copySshPublicKeyButton;
    button->setMinimumWidth(button->width());
    QString buttonSavedText = button->text();
    button->setText("Key Copied!");
    button->setEnabled(false);

    QTimer::singleShot(1000, this, [button, buttonSavedText]() {
        button->setText(buttonSavedText);
        button->setMinimumWidth(0);
        button->setEnabled(true);
    });
}
