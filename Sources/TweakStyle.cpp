#include "TweakStyle.h"

#include <QStyleOption>

TweakStyle::TweakStyle(QStyle* style) : QProxyStyle(style)
{
    // Nothing
}

int TweakStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget,
                            QStyleHintReturn* returnData) const
{
    if (hint == QStyle::SH_DialogButtonBox_ButtonsHaveIcons)
    {
        return 0;
    }

    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

void TweakStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter,
                                    const QWidget* widget) const
{
    // Indicator of a button opening a menu is tiny and overlaps the text, no space is reserved for it by QToolButton
    if (control == CC_ToolButton)
    {
        const QStyleOptionToolButton* buttonOption = qstyleoption_cast<const QStyleOptionToolButton*>(option);

        if (buttonOption && (buttonOption->features & QStyleOptionToolButton::HasMenu) &&
            !(buttonOption->features & QStyleOptionToolButton::MenuButtonPopup))
        {
            QStyleOptionToolButton plainButtonOption(*buttonOption);
            plainButtonOption.features &= ~QStyleOptionToolButton::HasMenu;
            QProxyStyle::drawComplexControl(control, &plainButtonOption, painter, widget);
            return;
        }
    }

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}
