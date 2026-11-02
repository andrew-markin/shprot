#include "Metrics.h"

#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFontInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace {

// Layout was designed with Ubuntu 10 pt font at 96 DPI, smaller fonts (e.g. Segoe UI 9 pt) keep designed values.
// Pixel size is used instead of line height, since the latter differs a lot between font families.
static const double ReferenceFontPixelSize = 13.0;

double getScaleFactor()
{
    return qMax(1.0, QFontInfo(QApplication::font()).pixelSize() / ReferenceFontPixelSize);
}

} // namespace

namespace Metrics {

int scaled(int value)
{
    return qRound(value * getScaleFactor());
}

int dialogMargin()
{
    return scaled(12);
}

int dialogSpacing()
{
    return scaled(12);
}

int groupSpacing()
{
    return scaled(10);
}

int titleSpacing()
{
    return scaled(2);
}

QDialogButtonBox* setupDialogLayout(QDialog* dialog, QVBoxLayout* contentLayout)
{
    int margin = dialogMargin();

    // Content is separated from buttons by doubled margin

    QWidget* topWidget = new QWidget();
    contentLayout->setSpacing(dialogSpacing());
    contentLayout->setContentsMargins(margin, margin, margin, 2 * margin);
    topWidget->setLayout(contentLayout);

    QDialogButtonBox* buttonBox = new QDialogButtonBox();

    QWidget* bottomWidget = new QWidget();
    QHBoxLayout* bottomWidgetLayout = new QHBoxLayout(bottomWidget);
    bottomWidgetLayout->setSpacing(dialogSpacing());
    bottomWidgetLayout->setContentsMargins(margin, 0, margin, margin);
    bottomWidgetLayout->addWidget(buttonBox);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(topWidget, 1);
    layout->addWidget(bottomWidget);

    return buttonBox;
}

} // namespace Metrics
