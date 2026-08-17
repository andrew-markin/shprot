#include "TweakStyle.h"

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
