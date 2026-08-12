#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QVariantMap>

class Preferences : public QObject
{
    Q_OBJECT

public:
    Preferences(const QString& path, QObject* parent = nullptr);

    bool hasValue(const QString& key);
    QVariant value(const QString& key);
    QVariant value(const QString& key, const QVariant& defaultValue);
    void setValue(const QString& key, const QVariant& value);
    void setValues(const QVariantMap& values);

signals:
    void changed(const QStringList& changes);

private:
    void save();

private:
    QString m_path;
    QVariantMap m_values;
};

#endif // PREFERENCES_H
