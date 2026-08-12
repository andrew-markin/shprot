#include "Preferences.h"

#include <QFile>
#include <QJsonDocument>
#include <QSaveFile>

Preferences::Preferences(const QString& path, QObject* parent) : QObject(parent), m_path(path)
{
    QFile file(m_path);

    if (!file.exists() || !file.open(QFile::ReadOnly))
    {
        return;
    }

    QByteArray data = file.readAll();

    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &jsonParseError);

    if (jsonParseError.error)
    {
        qCritical() << "Invalid settings file:" << jsonParseError.errorString();
    }
    else if (!jsonDocument.isObject())
    {
        qCritical() << "Invalid settings file structure";
    }
    else
    {
        m_values = jsonDocument.toVariant().toMap();
    }
}

void Preferences::save()
{
    QSaveFile file(m_path);

    if (!file.open(QIODevice::WriteOnly))
    {
        qCritical() << "Unable to open settings file for writing";
        return;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(m_values);
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    qint64 bytesWritten = file.write(data);

    if (bytesWritten == -1)
    {
        qWarning() << "Settings saving error:" << file.errorString();
        return;
    }

    if (bytesWritten != data.size())
    {
        qWarning() << "Settings were not saved completely: Only" << bytesWritten << "of" << data.size();
        return;
    }

    if (!file.commit())
    {
        qWarning() << "Unable to save settings file:" << file.errorString();
        return;
    }
}

bool Preferences::hasValue(const QString& key)
{
    return m_values.contains(key);
}

QVariant Preferences::value(const QString& key)
{
    return m_values.value(key);
}

QVariant Preferences::value(const QString& key, const QVariant& defaultValue)
{
    return m_values.value(key, defaultValue);
}

void Preferences::setValue(const QString& key, const QVariant& value)
{
    m_values.insert(key, value);
    save();
    emit changed({key});
}

void Preferences::setValues(const QVariantMap& values)
{
    m_values.insert(values);
    save();
    emit changed(values.keys());
}
