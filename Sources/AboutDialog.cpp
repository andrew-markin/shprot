#include "AboutDialog.h"

#include <QApplication>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QFile>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QStyle>
#include <QTabWidget>
#include <QVBoxLayout>

#include "Metrics.h"

namespace {

static const int IconSize = 64;
static const int LicenseTextColumns = 80; // License texts are formatted to fit 80 columns
static const int LicenseTextRows = 18;

// Project description comes from CMake, but lupdate doesn't expand macros, so the text is repeated here
// to be translated. Translation lookup falls back to the original text if the two ever differ.
[[maybe_unused]] static const char* const ProjectDescription =
    QT_TRANSLATE_NOOP("AboutDialog", "A lightweight GUI wrapper to manage a resilient proxy over SSH tunnel.");

QString readResource(const QString& path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return QString();
    }

    return QString::fromUtf8(file.readAll());
}

QString formatBuildTimestamp()
{
    QDateTime timestamp = QDateTime::fromString(PROJECT_BUILD_TIMESTAMP, Qt::ISODate);
    return timestamp.toUTC().toString("yyyy-MM-dd HH:mm 'UTC'");
}

QString getQtNotice()
{
    QString qtVersion = QT_VERSION_STR;
    QString qtMinorVersion = qtVersion.section('.', 0, 1);

    // Legal notice stays in English, as the license texts do
    return QString("%1 uses the Qt framework, version %2, licensed under the GNU Lesser General Public "
                   "License version 3 (LGPLv3). The text of the license is given below.\n\n"
                   "Qt is a trademark of The Qt Company Ltd. Copyright (C) The Qt Company Ltd. "
                   "and other contributors.\n\n"
                   "Qt libraries are dynamically linked and not modified. You may replace them with "
                   "your own compatible build of Qt. The complete corresponding source code of Qt is "
                   "available at:\n\n"
                   "https://download.qt.io/archive/qt/%3/%2/single/\n\n"
                   "Third-party components that Qt is built with are listed on the "
                   "\"Third-Party Notices\" tab.\n\n"
                   "%4")
        .arg(PROJECT_TITLE, qtVersion, qtMinorVersion, readResource(":/Licenses/LGPL-3.txt"));
}

} // namespace

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowFullscreenButtonHint);
    setWindowTitle(tr("About %1").arg(PROJECT_TITLE));

    QLabel* iconLabel = new QLabel();
    int iconSize = Metrics::scaled(IconSize);
    iconLabel->setPixmap(QIcon(":/Shprot.svg").pixmap(iconSize, iconSize));
    iconLabel->setAlignment(Qt::AlignTop);

    QLabel* titleLabel = new QLabel(PROJECT_TITLE);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSizeF(titleFont.pointSizeF() * 1.6);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QLabel* versionLabel = new QLabel(tr("Version %1, built on %2 with Qt %3")
                                      .arg(PROJECT_VERSION, formatBuildTimestamp(), QT_VERSION_STR));
    versionLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    QLabel* infoLabel = new QLabel(tr("%1<br><br><a href='%2'>GitHub repository</a>,&nbsp;"
                                      "<a href='%3'>LinkedIn profile</a><br><br>%4<br>"
                                      "Distributed under the GNU General Public License version 3.")
                                   .arg(tr(PROJECT_DESCRIPTION))
                                   .arg(PROJECT_GITHUB_REPOSITORY)
                                   .arg(PROJECT_LINKEDIN_PROFILE)
                                   .arg(PROJECT_COPYRIGHT));
    infoLabel->setTextFormat(Qt::RichText);
    infoLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::TextSelectableByMouse);
    infoLabel->setOpenExternalLinks(true);
    infoLabel->setWordWrap(true);

    // Title and version go together

    QVBoxLayout* headerTitleLayout = new QVBoxLayout();
    headerTitleLayout->setSpacing(Metrics::titleSpacing());
    headerTitleLayout->addWidget(titleLabel);
    headerTitleLayout->addWidget(versionLabel);

    QVBoxLayout* headerTextLayout = new QVBoxLayout();
    headerTextLayout->setSpacing(Metrics::groupSpacing());
    headerTextLayout->addLayout(headerTitleLayout);
    headerTextLayout->addWidget(infoLabel);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(Metrics::dialogSpacing());
    headerLayout->addWidget(iconLabel, 0, Qt::AlignTop);
    headerLayout->addLayout(headerTextLayout, 1);

    m_tabWidget = new QTabWidget();
    m_tabWidget->setDocumentMode(true);

    QPlainTextEdit* licenseEdit = addLicenseTab(tr("License"), readResource(":/Licenses/GPL-3.txt"));
    addLicenseTab("Qt", getQtNotice());
    addLicenseTab(tr("Third-Party Notices"), readResource(":/Licenses/ThirdParty.txt"));

    QVBoxLayout* contentLayout = new QVBoxLayout();
    contentLayout->addLayout(headerLayout);
    contentLayout->addWidget(m_tabWidget, 1);

    QDialogButtonBox* buttonBox = Metrics::setupDialogLayout(this, contentLayout);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    QPushButton* aboutQtButton = buttonBox->addButton(tr("About Qt…"), QDialogButtonBox::HelpRole);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(aboutQtButton, &QPushButton::clicked, this, [this]() { QMessageBox::aboutQt(this); });

    // Width is chosen to fit license texts without wrapping, height follows the content above them

    QFontMetrics licenseFontMetrics(licenseEdit->font());
    int licenseTextWidth = licenseFontMetrics.horizontalAdvance(QString(LicenseTextColumns, 'M'));
    int licenseTextHeight = licenseFontMetrics.lineSpacing() * LicenseTextRows;

    int scrollBarWidth = style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    int frameWidth = 2 * (licenseEdit->frameWidth() + licenseEdit->document()->documentMargin());

    licenseEdit->setMinimumSize(licenseTextWidth + scrollBarWidth + frameWidth, licenseTextHeight);
    setFixedSize(sizeHint());
}

void AboutDialog::open()
{
    if (!isVisible())
    {
        m_tabWidget->setCurrentIndex(0);
    }

    show();
    raise();
    activateWindow();
}

QPlainTextEdit* AboutDialog::addLicenseTab(const QString& title, const QString& text)
{
    QPlainTextEdit* edit = new QPlainTextEdit();
    edit->setReadOnly(true);
    edit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    edit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    edit->setPlainText(text);

    m_tabWidget->addTab(edit, title);
    return edit;
}
