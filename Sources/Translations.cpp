#include "Translations.h"

#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

namespace {

// Texts of Qt itself shown by the application: standard buttons (generic and GNOME variants) and context menus
// of text fields. Qt translations are not deployed, so these texts are translated together with the application
// texts. QT_LAYOUT_DIRECTION is translated to "RTL" for right-to-left languages to mirror the layout.
[[maybe_unused]] static const char* const QtTexts[] = {
    QT_TRANSLATE_NOOP("QGuiApplication", "QT_LAYOUT_DIRECTION"),

    QT_TRANSLATE_NOOP("QPlatformTheme", "OK"),
    QT_TRANSLATE_NOOP("QPlatformTheme", "Save"),
    QT_TRANSLATE_NOOP("QPlatformTheme", "Cancel"),
    QT_TRANSLATE_NOOP("QPlatformTheme", "Close"),
    QT_TRANSLATE_NOOP("QPlatformTheme", "Reset"),
    QT_TRANSLATE_NOOP("QPlatformTheme", "&Yes"),
    QT_TRANSLATE_NOOP("QPlatformTheme", "&No"),

    QT_TRANSLATE_NOOP("QGnomeTheme", "&OK"),
    QT_TRANSLATE_NOOP("QGnomeTheme", "&Save"),
    QT_TRANSLATE_NOOP("QGnomeTheme", "&Cancel"),
    QT_TRANSLATE_NOOP("QGnomeTheme", "&Close"),

    QT_TRANSLATE_NOOP("QLineEdit", "&Undo"),
    QT_TRANSLATE_NOOP("QLineEdit", "&Redo"),
    QT_TRANSLATE_NOOP("QLineEdit", "Cu&t"),
    QT_TRANSLATE_NOOP("QLineEdit", "&Copy"),
    QT_TRANSLATE_NOOP("QLineEdit", "&Paste"),
    QT_TRANSLATE_NOOP("QLineEdit", "Delete"),
    QT_TRANSLATE_NOOP("QLineEdit", "Select All"),

    QT_TRANSLATE_NOOP("QWidgetTextControl", "&Undo"),
    QT_TRANSLATE_NOOP("QWidgetTextControl", "&Redo"),
    QT_TRANSLATE_NOOP("QWidgetTextControl", "Cu&t"),
    QT_TRANSLATE_NOOP("QWidgetTextControl", "&Copy"),
    QT_TRANSLATE_NOOP("QWidgetTextControl", "&Paste"),
    QT_TRANSLATE_NOOP("QWidgetTextControl", "Delete"),
    QT_TRANSLATE_NOOP("QWidgetTextControl", "Select All"),
};

} // namespace

namespace Translations {

void install()
{
    // Lives as long as the application, translations are looked up on every tr() call
    static QTranslator translator;

    // Tries every UI language in order of preference, e.g. "pt_BR" before "pt"
    if (translator.load(QLocale(), PROJECT_NAME, "_", ":/Translations"))
    {
        QCoreApplication::installTranslator(&translator);
    }
}

} // namespace Translations
