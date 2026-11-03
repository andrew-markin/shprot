#include "PreferencesDialog.h"

#include <QCheckBox>
#include <QClipboard>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QStringList>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>

#include "Metrics.h"
#include "Preferences.h"
#include "PrivateKeyTextEdit.h"
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

QString formatProxyAddress(const QLineEdit* hostEdit, const QLineEdit* portEdit)
{
    QString host = hostEdit->text().trimmed();
    QString port = portEdit->text().trimmed();

    if (port.isEmpty())
    {
        port = portEdit->placeholderText();
    }

    if (host.contains(':') && !host.startsWith('['))
    {
        host = QString("[%1]").arg(host); // IPv6 address
    }

    return QString("%1:%2").arg(host, port);
}

QString unbreakableFingerprint(const QString& fingerprint)
{
    // <nobr> only prevents breaks at spaces, while line breaking rules also allow a break after "/" of base64,
    // so an invisible word joiner keeps the fingerprint on one line
    QString result = fingerprint;
    return result.replace('/', QString("/") + QChar(0x2060));
}

} // namespace

PreferencesDialog::PreferencesDialog(Preferences* preferences, QWidget* parent)
    : QDialog(parent), m_preferences(preferences)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowFullscreenButtonHint);
    setWindowTitle(tr("%1 Preferences").arg(PROJECT_TITLE));

    // SSH destination and port

    m_sshProxyTunnelCheckBox = new QCheckBox(tr("Enable SSH Proxy Tunnel"));

    QLabel* sshDestinationLabel = new QLabel(tr("SSH Destination (e.g. tunnel@myserver.com):"));
    sshDestinationLabel->setTextFormat(Qt::PlainText);

    QLabel* sshPortLabel = new QLabel(tr("Port:"));
    sshPortLabel->setTextFormat(Qt::PlainText);

    m_sshDestinationEdit = new QLineEdit();

    m_sshPortEdit = new QLineEdit();
    m_sshPortEdit->setPlaceholderText("22");

    QGridLayout* sshDestinationAndPortLayout = new QGridLayout();
    sshDestinationAndPortLayout->setSpacing(Metrics::groupSpacing());
    sshDestinationAndPortLayout->setColumnStretch(0, 1);
    sshDestinationAndPortLayout->addWidget(sshDestinationLabel, 0, 0);
    sshDestinationAndPortLayout->addWidget(sshPortLabel, 0, 1);
    sshDestinationAndPortLayout->addWidget(m_sshDestinationEdit, 1, 0);
    sshDestinationAndPortLayout->addWidget(m_sshPortEdit, 1, 1);

    // Trust check

    m_trustCheckButton = new QToolButton();
    m_trustCheckButton->setText(tr("Run Trust Check"));

    QLabel* trustCheckLabel = new QLabel(tr("Verifies the server's identity."));
    trustCheckLabel->setTextFormat(Qt::PlainText);

    QHBoxLayout* trustCheckLayout = new QHBoxLayout();
    trustCheckLayout->setSpacing(Metrics::groupSpacing());
    trustCheckLayout->addWidget(m_trustCheckButton);
    trustCheckLayout->addWidget(trustCheckLabel);

    // SSH private key

    QLabel* sshPrivateKeyLabel = new QLabel(tr("SSH Private Key:"));
    sshPrivateKeyLabel->setTextFormat(Qt::PlainText);

    QToolButton* generateSshPrivateKeyButton = new QToolButton();
    generateSshPrivateKeyButton->setText(tr("Generate"));

    QToolButton* importSshPrivateKeyButton = new QToolButton();
    importSshPrivateKeyButton->setText(tr("Import…"));

    m_copySshPublicKeyButton = new QToolButton();
    m_copySshPublicKeyButton->setText(tr("Copy Public Key"));

    QHBoxLayout* sshPrivateKeyLabelLayout = new QHBoxLayout();
    sshPrivateKeyLabelLayout->setSpacing(Metrics::groupSpacing());
    sshPrivateKeyLabelLayout->addWidget(sshPrivateKeyLabel);
    sshPrivateKeyLabelLayout->addWidget(generateSshPrivateKeyButton);
    sshPrivateKeyLabelLayout->addWidget(importSshPrivateKeyButton);
    sshPrivateKeyLabelLayout->addWidget(m_copySshPublicKeyButton);

    m_sshPrivateKeyEdit = new PrivateKeyTextEdit();

    // Local SOCKS5 proxy

    QLabel* localSocks5ProxyAddressLabel = new QLabel(tr("Local SOCKS5 Proxy Address (e.g. localhost or 0.0.0.0):"));
    localSocks5ProxyAddressLabel->setTextFormat(Qt::PlainText);

    QLabel* localSocks5ProxyPortLabel = new QLabel(tr("Port:"));
    localSocks5ProxyPortLabel->setTextFormat(Qt::PlainText);

    m_localSocks5ProxyHostEdit = new QLineEdit();
    m_localSocks5ProxyHostEdit->setPlaceholderText("localhost");

    m_localSocks5ProxyPortEdit = new QLineEdit();
    m_localSocks5ProxyPortEdit->setPlaceholderText("1080");

    QGridLayout* localSocks5ProxyAddressLayout = new QGridLayout();
    localSocks5ProxyAddressLayout->setSpacing(Metrics::groupSpacing());
    localSocks5ProxyAddressLayout->setColumnStretch(0, 1);
    localSocks5ProxyAddressLayout->addWidget(localSocks5ProxyAddressLabel, 0, 0);
    localSocks5ProxyAddressLayout->addWidget(localSocks5ProxyPortLabel, 0, 1);
    localSocks5ProxyAddressLayout->addWidget(m_localSocks5ProxyHostEdit, 1, 0);
    localSocks5ProxyAddressLayout->addWidget(m_localSocks5ProxyPortEdit, 1, 1);

    // Local HTTP proxy

    m_localHttpProxyCheckBox = new QCheckBox(tr("Enable Local HTTP Proxy"));

    QLabel* localHttpProxyAddressLabel = new QLabel(tr("Local HTTP Proxy Address (e.g. localhost or 0.0.0.0):"));
    localHttpProxyAddressLabel->setTextFormat(Qt::PlainText);

    QLabel* localHttpProxyPortLabel = new QLabel(tr("Port:"));
    localHttpProxyPortLabel->setTextFormat(Qt::PlainText);

    m_localHttpProxyHostEdit = new QLineEdit();
    m_localHttpProxyHostEdit->setPlaceholderText("localhost");

    m_localHttpProxyPortEdit = new QLineEdit();
    m_localHttpProxyPortEdit->setPlaceholderText("8080");

    QWidget* localHttpProxyAddressWidget = new QWidget();
    localHttpProxyAddressWidget->setEnabled(false);

    QGridLayout* localHttpProxyAddressLayout = new QGridLayout(localHttpProxyAddressWidget);
    localHttpProxyAddressLayout->setSpacing(Metrics::groupSpacing());
    localHttpProxyAddressLayout->setContentsMargins(0, 0, 0, 0);
    localHttpProxyAddressLayout->setColumnStretch(0, 1);
    localHttpProxyAddressLayout->addWidget(localHttpProxyAddressLabel, 0, 0);
    localHttpProxyAddressLayout->addWidget(localHttpProxyPortLabel, 0, 1);
    localHttpProxyAddressLayout->addWidget(m_localHttpProxyHostEdit, 1, 0);
    localHttpProxyAddressLayout->addWidget(m_localHttpProxyPortEdit, 1, 1);

    // Everything below the tunnel check box is enabled with it, groups are separated by extra spacing

    QWidget* sshProxyTunnelWidget = new QWidget();
    sshProxyTunnelWidget->setEnabled(false);

    QVBoxLayout* sshProxyTunnelWidgetLayout = new QVBoxLayout(sshProxyTunnelWidget);
    sshProxyTunnelWidgetLayout->setSpacing(Metrics::groupSpacing());
    sshProxyTunnelWidgetLayout->setContentsMargins(0, 0, 0, 0);
    sshProxyTunnelWidgetLayout->addLayout(sshDestinationAndPortLayout);
    sshProxyTunnelWidgetLayout->addLayout(trustCheckLayout);
    sshProxyTunnelWidgetLayout->addSpacing(Metrics::groupSpacing());
    sshProxyTunnelWidgetLayout->addLayout(sshPrivateKeyLabelLayout);
    sshProxyTunnelWidgetLayout->addWidget(m_sshPrivateKeyEdit);
    sshProxyTunnelWidgetLayout->addSpacing(Metrics::groupSpacing());
    sshProxyTunnelWidgetLayout->addLayout(localSocks5ProxyAddressLayout);
    sshProxyTunnelWidgetLayout->addSpacing(Metrics::groupSpacing());
    sshProxyTunnelWidgetLayout->addWidget(m_localHttpProxyCheckBox);
    sshProxyTunnelWidgetLayout->addWidget(localHttpProxyAddressWidget);

    QVBoxLayout* contentLayout = new QVBoxLayout();
    contentLayout->addWidget(m_sshProxyTunnelCheckBox);
    contentLayout->addWidget(sshProxyTunnelWidget);

    QDialogButtonBox* buttonBox = Metrics::setupDialogLayout(this, contentLayout);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Reset | QDialogButtonBox::Save);

    // Tab order follows visual order, not the order in which widgets were created and placed

    QList<QWidget*> tabOrder = {m_sshProxyTunnelCheckBox, m_sshDestinationEdit, m_sshPortEdit,
                                m_trustCheckButton, generateSshPrivateKeyButton, importSshPrivateKeyButton,
                                m_copySshPublicKeyButton, m_sshPrivateKeyEdit, m_localSocks5ProxyHostEdit,
                                m_localSocks5ProxyPortEdit, m_localHttpProxyCheckBox, m_localHttpProxyHostEdit,
                                m_localHttpProxyPortEdit};

    for (int i = 1; i < tabOrder.size(); i++)
    {
        setTabOrder(tabOrder[i - 1], tabOrder[i]);
    }

    resize(sizeHint());

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

    connect(m_sshProxyTunnelCheckBox, &QCheckBox::toggled, sshProxyTunnelWidget, &QWidget::setEnabled);

    static const QRegularExpression SshDestinationRegEx(R"(^[a-zA-Z0-9._-]+\@[a-zA-Z0-9.-]+$)");
    QRegularExpressionValidator* sshDestinationValidator = new QRegularExpressionValidator(SshDestinationRegEx, this);
    m_sshDestinationEdit->setValidator(sshDestinationValidator);
    connectHighlightReset(m_sshDestinationEdit);

    static const QRegularExpression PortRegEx(R"(^([0-9]{0,5})$)");
    QRegularExpressionValidator* portValidator = new QRegularExpressionValidator(PortRegEx, this);

    m_sshPortEdit->setValidator(portValidator);
    connectHighlightReset(m_sshPortEdit);

    connectHighlightReset(m_sshPrivateKeyEdit);

    static const QRegularExpression HostRegEx(R"(^(\[[0-9a-fA-F:]+\]|[a-zA-Z0-9.-]*)$)");
    QRegularExpressionValidator* hostValidator = new QRegularExpressionValidator(HostRegEx, this);

    m_localSocks5ProxyHostEdit->setValidator(hostValidator);
    connectHighlightReset(m_localSocks5ProxyHostEdit);

    m_localSocks5ProxyPortEdit->setValidator(portValidator);
    connectHighlightReset(m_localSocks5ProxyPortEdit);

    connect(m_trustCheckButton, &QToolButton::clicked, this, &PreferencesDialog::handleTrustCheckButtonClick);
    connect(generateSshPrivateKeyButton, &QToolButton::clicked, this, &PreferencesDialog::generateSshPrivateKey);
    connect(importSshPrivateKeyButton, &QToolButton::clicked, this, &PreferencesDialog::importSshPrivateKey);
    connect(m_copySshPublicKeyButton, &QToolButton::clicked, this, &PreferencesDialog::copySshPublicKey);

    connect(m_localHttpProxyCheckBox, &QCheckBox::toggled, localHttpProxyAddressWidget, &QWidget::setEnabled);

    m_localHttpProxyHostEdit->setValidator(hostValidator);
    connectHighlightReset(m_localHttpProxyHostEdit);

    m_localHttpProxyPortEdit->setValidator(portValidator);
    connectHighlightReset(m_localHttpProxyPortEdit);

    QPushButton* resetButton = buttonBox->button(QDialogButtonBox::Reset);
    connect(resetButton, &QPushButton::clicked, this, &PreferencesDialog::reset);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &PreferencesDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PreferencesDialog::reject);
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
        messageBox.setWindowTitle(tr("Preferences Error"));
        messageBox.setTextFormat(Qt::RichText);
        messageBox.setText(tr("One or more fields are invalid:<br><br>%1")
                           .arg(errorLines.join("<br>")));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();

        return;
    }

    if (!confirmExposure())
    {
        return;
    }

    if (m_sshProxyTunnelCheckBox->isChecked() && !runTrustCheck(false))
    {
        return;
    }

    QVariantMap sshProxyTunnel({{"enabled", m_sshProxyTunnelCheckBox->isChecked()},
                                {"sshDestination", m_sshDestinationEdit->text()},
                                {"sshPort", m_sshPortEdit->text()},
                                {"sshPrivateKey", m_sshPrivateKeyEdit->toPlainText()},
                                {"localSocks5ProxyHost", m_localSocks5ProxyHostEdit->text()},
                                {"localSocks5ProxyPort", m_localSocks5ProxyPortEdit->text()},
                                {"localHttpProxyEnabled", m_localHttpProxyCheckBox->isChecked()},
                                {"localHttpProxyHost", m_localHttpProxyHostEdit->text()},
                                {"localHttpProxyPort", m_localHttpProxyPortEdit->text()}});

    m_preferences->setValue("sshProxyTunnel", sshProxyTunnel);

    QDialog::accept();
}

void PreferencesDialog::reject()
{
    if (hasChanges() && (QMessageBox::question(this, tr("Unsaved Changes"),
                                               tr("You have unsaved changes. Do you want to exit without saving?"),
                                               QMessageBox::Yes | QMessageBox::No) == QMessageBox::No))
    {
        return;
    }

    QDialog::reject();
}

void PreferencesDialog::reset()
{
    QVariantMap sshProxyTunnel = m_preferences->value("sshProxyTunnel").toMap();

    m_sshProxyTunnelCheckBox->setChecked(sshProxyTunnel.value("enabled", false).toBool());
    m_sshDestinationEdit->setText(sshProxyTunnel.value("sshDestination").toString());
    m_sshPortEdit->setText(sshProxyTunnel.value("sshPort").toString());
    m_sshPrivateKeyEdit->setPlainText(sshProxyTunnel.value("sshPrivateKey").toString());
    m_localSocks5ProxyHostEdit->setText(sshProxyTunnel.value("localSocks5ProxyHost").toString());
    m_localSocks5ProxyPortEdit->setText(sshProxyTunnel.value("localSocks5ProxyPort").toString());
    m_localHttpProxyCheckBox->setChecked(sshProxyTunnel.value("localHttpProxyEnabled", false).toBool());
    m_localHttpProxyHostEdit->setText(sshProxyTunnel.value("localHttpProxyHost").toString());
    m_localHttpProxyPortEdit->setText(sshProxyTunnel.value("localHttpProxyPort").toString());

    validate();
}

void PreferencesDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    clearChildrenFocus();
    activateWindow();
}

void PreferencesDialog::hideEvent(QHideEvent* event)
{
    m_preferences->setValue("preferencesDialogGeometry", saveGeometry().toBase64());
    QDialog::hideEvent(event);
}

void PreferencesDialog::mousePressEvent(QMouseEvent* event)
{
    clearChildrenFocus();
    QWidget::mousePressEvent(event);
}

void PreferencesDialog::clearChildrenFocus()
{
    QWidget* focusWidget = QDialog::focusWidget();

    if (focusWidget)
    {
        focusWidget->clearFocus();
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

bool PreferencesDialog::hasChanges() const
{
    QVariantMap sshProxyTunnel = m_preferences->value("sshProxyTunnel").toMap();
    return (m_sshProxyTunnelCheckBox->isChecked() != sshProxyTunnel.value("enabled", false).toBool()) ||
            (m_sshDestinationEdit->text() != sshProxyTunnel.value("sshDestination").toString()) ||
            (m_sshPortEdit->text() != sshProxyTunnel.value("sshPort").toString()) ||
            (m_sshPrivateKeyEdit->toPlainText() != sshProxyTunnel.value("sshPrivateKey").toString()) ||
            (m_localSocks5ProxyHostEdit->text() != sshProxyTunnel.value("localSocks5ProxyHost").toString()) ||
            (m_localSocks5ProxyPortEdit->text() != sshProxyTunnel.value("localSocks5ProxyPort").toString()) ||
            (m_localHttpProxyCheckBox->isChecked() != sshProxyTunnel.value("localHttpProxyEnabled", false).toBool()) ||
            (m_localHttpProxyHostEdit->text() != sshProxyTunnel.value("localHttpProxyHost").toString()) ||
            (m_localHttpProxyPortEdit->text() != sshProxyTunnel.value("localHttpProxyPort").toString());
}

bool PreferencesDialog::confirmExposure()
{
    QVariantMap sshProxyTunnel = m_preferences->value("sshProxyTunnel").toMap();

    bool tunnelEnabled = m_sshProxyTunnelCheckBox->isChecked();
    bool httpProxyEnabled = tunnelEnabled && m_localHttpProxyCheckBox->isChecked();

    bool savedTunnelEnabled = sshProxyTunnel.value("enabled", false).toBool();
    bool savedHttpProxyEnabled = savedTunnelEnabled && sshProxyTunnel.value("localHttpProxyEnabled", false).toBool();

    // Ask only when the proxy becomes exposed, so already accepted settings are not questioned again
    auto becomesExposed = [](bool enabled, const QString& host, bool savedEnabled, const QString& savedHost) {
        bool wasExposed = savedEnabled && Utilities::hostIsExposed(savedHost) && (host == savedHost);
        return enabled && Utilities::hostIsExposed(host) && !wasExposed;
    };

    bool socks5ProxyBecomesExposed = becomesExposed(tunnelEnabled, m_localSocks5ProxyHostEdit->text(),
                                                    savedTunnelEnabled,
                                                    sshProxyTunnel.value("localSocks5ProxyHost").toString());

    bool httpProxyBecomesExposed = becomesExposed(httpProxyEnabled, m_localHttpProxyHostEdit->text(),
                                                  savedHttpProxyEnabled,
                                                  sshProxyTunnel.value("localHttpProxyHost").toString());

    QString socks5ProxyAddress = formatProxyAddress(m_localSocks5ProxyHostEdit, m_localSocks5ProxyPortEdit);
    QString httpProxyAddress = formatProxyAddress(m_localHttpProxyHostEdit, m_localHttpProxyPortEdit);

    // Whole sentences for each case, so they can be translated naturally
    QString text;

    if (socks5ProxyBecomesExposed && httpProxyBecomesExposed)
    {
        text = tr("Local SOCKS5 proxy at %1 and HTTP proxy at %2 will be accessible from other computers "
                  "without authentication.").arg(socks5ProxyAddress, httpProxyAddress);
    }
    else if (socks5ProxyBecomesExposed)
    {
        text = tr("Local SOCKS5 proxy at %1 will be accessible from other computers "
                  "without authentication.").arg(socks5ProxyAddress);
    }
    else if (httpProxyBecomesExposed)
    {
        text = tr("Local HTTP proxy at %1 will be accessible from other computers "
                  "without authentication.").arg(httpProxyAddress);
    }
    else
    {
        return true;
    }

    QMessageBox messageBox(this);
    messageBox.setWindowTitle(tr("Proxy Accessible from Network"));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setText(text);
    messageBox.setInformativeText(tr("Anyone who can reach this address can use your SSH tunnel. "
                                     "Do you want to save these settings anyway?"));
    messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);

    return messageBox.exec() == QMessageBox::Save;
}

QStringList PreferencesDialog::validateSshDestinationAndPort()
{
    QStringList errors;

    setWidgetHighlighted(m_sshDestinationEdit, false);
    setWidgetHighlighted(m_sshPortEdit, false);

    // SSH Destination

    if (!m_sshDestinationEdit->hasAcceptableInput())
    {
        errors.append(tr("Invalid SSH Destination"));
        setWidgetHighlighted(m_sshDestinationEdit, true);
    }

    // SSH Port

    if (!m_sshPortEdit->hasAcceptableInput() || !portStringIsAcceptable(m_sshPortEdit->text(), true))
    {
        errors.append(tr("Invalid SSH Port"));
        setWidgetHighlighted(m_sshPortEdit, true);
    }

    return errors;
}

QStringList PreferencesDialog::validate()
{
    QStringList errors;

    setWidgetHighlighted(m_sshDestinationEdit, false);
    setWidgetHighlighted(m_sshPortEdit, false);
    setWidgetHighlighted(m_sshPrivateKeyEdit, false);
    setWidgetHighlighted(m_localSocks5ProxyHostEdit, false);
    setWidgetHighlighted(m_localSocks5ProxyPortEdit, false);
    setWidgetHighlighted(m_localHttpProxyHostEdit, false);
    setWidgetHighlighted(m_localHttpProxyPortEdit, false);

    if (!m_sshProxyTunnelCheckBox->isChecked())
    {
        return errors;
    }

    errors = validateSshDestinationAndPort();

    // SSH Private Key

    if (!Utilities::sshPrivateKeyLooksValid(m_sshPrivateKeyEdit->toPlainText()))
    {
        errors.append(tr("Invalid SSH Private Key"));
        setWidgetHighlighted(m_sshPrivateKeyEdit, true);
    }
    else if (Utilities::sshPrivateKeyIsEncrypted(m_sshPrivateKeyEdit->toPlainText()))
    {
        errors.append(Utilities::getEncryptedPrivateKeyMessage());
        setWidgetHighlighted(m_sshPrivateKeyEdit, true);
    }

    // Local SOCKS5 Proxy Host

    if (!m_localSocks5ProxyHostEdit->hasAcceptableInput())
    {
        errors.append(tr("Invalid Local SOCKS5 Proxy Host"));
        setWidgetHighlighted(m_localSocks5ProxyHostEdit, true);
    }

    // Local SOCKS5 Proxy Port

    if (!m_localSocks5ProxyPortEdit->hasAcceptableInput() ||
        !portStringIsAcceptable(m_localSocks5ProxyPortEdit->text()))
    {
        errors.append(tr("Invalid Local SOCKS5 Proxy Port"));
        setWidgetHighlighted(m_localSocks5ProxyPortEdit, true);
    }

    // Local HTTP Proxy

    if (m_localHttpProxyCheckBox->isChecked())
    {
        // Local HTTP Proxy Host

        if (!m_localHttpProxyHostEdit->hasAcceptableInput())
        {
            errors.append(tr("Invalid Local HTTP Proxy Host"));
            setWidgetHighlighted(m_localHttpProxyHostEdit, true);
        }

        // Local HTTP Proxy Port

        if (!m_localHttpProxyPortEdit->hasAcceptableInput() ||
            !portStringIsAcceptable(m_localHttpProxyPortEdit->text()))
        {
            errors.append(tr("Invalid Local HTTP Proxy Port"));
            setWidgetHighlighted(m_localHttpProxyPortEdit, true);
        }
    }

    return errors;
}

bool PreferencesDialog::runTrustCheck(bool forceProbe)
{
    QStringList errors = validateSshDestinationAndPort();

    if (!errors.isEmpty())
    {
        QMessageBox::warning(this, tr("Trust Check Error"), tr("Invalid SSH Destination or Port"), QMessageBox::Ok);
        return false;
    }

    QString address = Utilities::getAddressFromSshDestination(m_sshDestinationEdit->text());

    QString knownHostRecord = Utilities::getKnownHostRecord(address);
    bool hostIsKnown = !knownHostRecord.isEmpty();

    if (hostIsKnown && !forceProbe)
    {
        return true;
    }

    QString missingSshMessage = Utilities::getMissingSshToolMessage("ssh");

    if (!missingSshMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("Trust Check Error"), missingSshMessage, QMessageBox::Ok);
        return false;
    }

    quint16 port = Utilities::parsePort(m_sshPortEdit->text(), 22);
    QString probeHostRecord = Utilities::probeActualHostRecord(address, port);

    if (probeHostRecord.isEmpty())
    {
        QMessageBox::warning(this, tr("Trust Check Error"), tr("Host probe failed"), QMessageBox::Ok);
        return false;
    }

    auto [probeKeyType, probeKeyFingerprint] = Utilities::getKeyTypeAndFingerprint(probeHostRecord);

    if (!hostIsKnown)
    {
        QMessageBox messageBox(this);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setWindowTitle(tr("Unknown Server Identity"));
        messageBox.setTextFormat(Qt::RichText);
        messageBox.setText(
                    tr("The authenticity of host <b>%1</b> can't be established.<br><br>"
                       "%2 key fingerprint is:<br><nobr>SHA256:%3.</nobr>")
                    .arg(address)
                    .arg(Utilities::getKeyTypeLabel(probeKeyType))
                    .arg(unbreakableFingerprint(probeKeyFingerprint)));
        messageBox.setInformativeText(tr("Are you sure you want to continue connecting and trust this server?"));
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
        messageBox.setWindowTitle(tr("Remote Host Identification Has Changed"));
        messageBox.setTextFormat(Qt::RichText);
        messageBox.setText(
                    tr("The server key fingerprint for <b>%1</b> does not match the previously saved value. "
                       "This could mean someone is intercepting your connection (Man-in-the-Middle attack) "
                       "or the server administrator just reinstalled the OS.<br><br>"
                       "Saved %2 key fingerprint is:<br><nobr>SHA256:%3.</nobr><br><br>"
                       "Received %4 key fingerprint is:<br><nobr>SHA256:%5.</nobr>")
                    .arg(address)
                    .arg(Utilities::getKeyTypeLabel(knownKeyType))
                    .arg(unbreakableFingerprint(knowKeyFingerprint))
                    .arg(Utilities::getKeyTypeLabel(probeKeyType))
                    .arg(unbreakableFingerprint(probeKeyFingerprint)));
        messageBox.setInformativeText(tr("If you are certain this change is legitimate, you can update the trusted key."));
        messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        QAbstractButton* saveButton = messageBox.button(QMessageBox::Save);
        saveButton->setText(tr("Update Key and Trust"));
        saveButton->setMinimumWidth(Metrics::scaled(140));
        messageBox.setDefaultButton(QMessageBox::Cancel);
        messageBox.setEscapeButton(QMessageBox::Cancel);

        if (messageBox.exec() == QMessageBox::Save)
        {
            Utilities::setKnownHostRecord(address, probeHostRecord);
            return true;
        }

        return false;
    }

    QMessageBox::information(this, tr("Trust Check Succeeded"), tr("Server identity verified successfully."),
                             QMessageBox::Ok);
    return true;
}

void PreferencesDialog::handleTrustCheckButtonClick()
{
    QToolButton* button = m_trustCheckButton;
    QString buttonSavedText = button->text();
    button->setMinimumWidth(button->width());
    button->setText(tr("Checking Trust"));
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
    QString missingSshKeygenMessage = Utilities::getMissingSshToolMessage("ssh-keygen");

    if (!missingSshKeygenMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("SSH Private Key Generation Error"), missingSshKeygenMessage, QMessageBox::Ok);
        return;
    }

    m_sshPrivateKeyEdit->setPlainText(Utilities::generateSshPrivateKey());
    m_sshPrivateKeyEdit->reveal();
}

void PreferencesDialog::importSshPrivateKey()
{
    QFileDialog fileDialog(this);

    fileDialog.setWindowTitle(tr("Open SSH Private Key"));
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    fileDialog.setDirectory(m_preferences->value("sshPrivateKeyImportDirectory", QDir::homePath()).toString());

    if (fileDialog.exec() != QFileDialog::Accepted)
    {
        return;
    }

    m_preferences->setValue("sshPrivateKeyImportDirectory", fileDialog.directory().canonicalPath());

    QFile selectedFile(fileDialog.selectedFiles().first());
    QString importError = tr("Invalid SSH Private Key file");

    if ((selectedFile.size() <= 32768) && selectedFile.open(QIODevice::ReadOnly))
    {
        QString privateKey = QString::fromUtf8(selectedFile.readAll());

        if (Utilities::sshPrivateKeyLooksValid(privateKey))
        {
            if (!Utilities::sshPrivateKeyIsEncrypted(privateKey))
            {
                m_sshPrivateKeyEdit->setPlainText(privateKey);
                m_sshPrivateKeyEdit->reveal();
                return;
            }

            importError = Utilities::getEncryptedPrivateKeyMessage();
        }
    }

    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("SSH Private Key Import Error"));
    messageBox.setText(importError);
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.exec();
}

void PreferencesDialog::copySshPublicKey()
{
    QString missingSshKeygenMessage = Utilities::getMissingSshToolMessage("ssh-keygen");

    if (!missingSshKeygenMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("SSH Public Key Error"), missingSshKeygenMessage, QMessageBox::Ok);
        return;
    }

    QString privateKey = m_sshPrivateKeyEdit->toPlainText();

    if (Utilities::sshPrivateKeyIsEncrypted(privateKey))
    {
        QMessageBox::warning(this, tr("SSH Public Key Error"), Utilities::getEncryptedPrivateKeyMessage(), QMessageBox::Ok);
        return;
    }

    QString publicKey = Utilities::getSshPublicKey(privateKey);

    if (publicKey.isEmpty())
    {
        QMessageBox::warning(this, tr("SSH Public Key Error"), tr("Invalid SSH Private Key"), QMessageBox::Ok);
        return;
    }

    QGuiApplication::clipboard()->setText(publicKey);

    QToolButton* button = m_copySshPublicKeyButton;
    button->setMinimumWidth(button->width());
    QString buttonSavedText = button->text();
    button->setText(tr("Key Copied!"));
    button->setEnabled(false);

    QTimer::singleShot(1000, this, [button, buttonSavedText]() {
        button->setText(buttonSavedText);
        button->setMinimumWidth(0);
        button->setEnabled(true);
    });
}
