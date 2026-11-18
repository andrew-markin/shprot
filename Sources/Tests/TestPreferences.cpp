#include <QFile>
#include <QRegularExpression>
#include <QSaveFile>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>

#ifdef Q_OS_UNIX
#include <QFileDevice>
#endif

#include "Preferences.h"

class TestPreferences : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void valuesRoundTrip();
    void changedIsEmitted();
    void corruptedFileFallsBackToEmpty();
    void notObjectFileFallsBackToEmpty();

#ifdef Q_OS_UNIX
    void savedFileIsReadableByOwnerOnly();
#endif

private:
    QString path() const;

private:
    QTemporaryDir* m_temporaryDir = nullptr;
};

void TestPreferences::init()
{
    m_temporaryDir = new QTemporaryDir();
    QVERIFY(m_temporaryDir->isValid());
}

void TestPreferences::cleanup()
{
    delete m_temporaryDir;
}

QString TestPreferences::path() const
{
    return m_temporaryDir->filePath("Preferences.json");
}

void TestPreferences::valuesRoundTrip()
{
    QVariantMap values = {
        {"showNotifications", false},
        {"sshProxyTunnel", QVariantMap({{"enabled", true}, {"sshPort", "2222"}})},
        {"nested", QVariantList({1, 2.5, "three", true})}
    };

    {
        Preferences preferences(path());
        preferences.setValues(values);
    }

    // A new instance reads what the previous one saved
    Preferences again(path());

    QCOMPARE(again.value("showNotifications").toBool(), false);
    QCOMPARE(again.value("sshProxyTunnel").toMap(), values.value("sshProxyTunnel").toMap());
    QCOMPARE(again.value("nested").toList(), values.value("nested").toList());
    QCOMPARE(again.hasValue("sshProxyTunnel"), true);
    QCOMPARE(again.hasValue("missing"), false);
    QCOMPARE(again.value("missing", "default").toString(), QString("default"));
}

void TestPreferences::changedIsEmitted()
{
    Preferences preferences(path());
    QSignalSpy changedSpy(&preferences, &Preferences::changed);

    preferences.setValue("key", 1);

    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(changedSpy.first().first().toString(), QString("key"));
}

void TestPreferences::corruptedFileFallsBackToEmpty()
{
    QSaveFile file(path());
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("{ not a json");
    QVERIFY(file.commit());

    QTest::ignoreMessage(QtCriticalMsg, QRegularExpression("Invalid settings file: .*"));

    Preferences preferences(path());

    QCOMPARE(preferences.value("showNotifications", true).toBool(), true); // Defaults are used
    preferences.setValue("key", 1); // Saving recreates the file from scratch

    Preferences again(path());
    QCOMPARE(again.value("key").toInt(), 1);
}

void TestPreferences::notObjectFileFallsBackToEmpty()
{
    QSaveFile file(path());
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("[1, 2, 3]");
    QVERIFY(file.commit());

    QTest::ignoreMessage(QtCriticalMsg, "Invalid settings file structure");

    Preferences preferences(path());

    QCOMPARE(preferences.value("key", "default").toString(), QString("default"));
}

#ifdef Q_OS_UNIX
// The settings contain the private key, the file must never become readable by others
void TestPreferences::savedFileIsReadableByOwnerOnly()
{
    Preferences preferences(path());
    preferences.setValue("key", "value");

    // On Unix permissions() reports the owner twice, as Owner and as User
    QFile::Permissions expected = QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser | QFile::WriteUser;
    QCOMPARE(QFile(path()).permissions(), expected);
}
#endif

QTEST_GUILESS_MAIN(TestPreferences)

#include "TestPreferences.moc"
