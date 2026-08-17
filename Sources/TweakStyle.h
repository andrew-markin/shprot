#ifndef TWEAKSTYLE_H
#define TWEAKSTYLE_H

#include <QProxyStyle>

class TweakStyle : public QProxyStyle
{
public:
    explicit TweakStyle(QStyle* style = nullptr);

    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr,
                  QStyleHintReturn* returnData = nullptr) const override;
};

#endif // TWEAKSTYLE_H
