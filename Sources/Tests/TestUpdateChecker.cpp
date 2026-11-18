#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>

#include "Preferences.h"
#include "UpdateChecker.h"

class TestUpdateChecker : public QObject
{
    Q_OBJECT

private slots:
    void parseTagVersion_data();
    void parseTagVersion();

    void isNewerVersion_data();
    void isNewerVersion();

    void savedVersionIsOffered();
    void versionEqualToCurrentIsNotOffered();
    void invalidSavedVersionIsNotOffered();

private:
    QString preferencesPath() const;

private:
    QTemporaryDir m_temporaryDir;
};

// The checker restores the version found before restart from the preferences
void TestUpdateChecker::savedVersionIsOffered()
{
    Preferences preferences(preferencesPath());
    preferences.setValue("latestVersion", "9.9.9");

    UpdateChecker checker(&preferences);

    QCOMPARE(checker.newVersion(), QString("9.9.9"));
    QCOMPARE(checker.newVersionUrl(), QUrl(QString(PROJECT_GITHUB_REPOSITORY) + "/releases/tag/v9.9.9"));
}

void TestUpdateChecker::versionEqualToCurrentIsNotOffered()
{
    Preferences preferences(preferencesPath());
    preferences.setValue("latestVersion", "v" PROJECT_VERSION);

    UpdateChecker checker(&preferences);

    QCOMPARE(checker.newVersion(), QString());
}

void TestUpdateChecker::invalidSavedVersionIsNotOffered()
{
    Preferences preferences(preferencesPath());
    preferences.setValue("latestVersion", "garbage");

    UpdateChecker checker(&preferences);

    QCOMPARE(checker.newVersion(), QString());
}

QString TestUpdateChecker::preferencesPath() const
{
    return m_temporaryDir.filePath("Preferences.json");
}

void TestUpdateChecker::parseTagVersion_data()
{
    QTest::addColumn<QString>("tag");
    QTest::addColumn<QString>("expected");

    QTest::newRow("major and minor") << "v1.6" << "1.6";
    QTest::newRow("with patch") << "v1.6.1" << "1.6.1";
    QTest::newRow("no prefix") << "1.6" << "";
    QTest::newRow("major only") << "v2" << "";
    QTest::newRow("suffix") << "v1.7-beta" << "";
    QTest::newRow("path") << "v1.7/../x" << "";
    QTest::newRow("empty") << "" << "";
}

void TestUpdateChecker::parseTagVersion()
{
    QFETCH(QString, tag);
    QFETCH(QString, expected);

    QCOMPARE(UpdateChecker::parseTagVersion(tag), expected);
}

void TestUpdateChecker::isNewerVersion_data()
{
    QTest::addColumn<QString>("version");
    QTest::addColumn<QString>("currentVersion");
    QTest::addColumn<bool>("expected");

    QTest::newRow("newer minor") << "1.7" << "1.6" << true;
    QTest::newRow("numeric, not textual") << "1.10" << "1.9" << true;
    QTest::newRow("newer major") << "2.0" << "1.6" << true;
    QTest::newRow("patch") << "1.6.1" << "1.6" << true;
    QTest::newRow("same") << "1.6" << "1.6" << false;
    QTest::newRow("older") << "1.5" << "1.6" << false;
}

void TestUpdateChecker::isNewerVersion()
{
    QFETCH(QString, version);
    QFETCH(QString, currentVersion);
    QFETCH(bool, expected);

    QCOMPARE(UpdateChecker::isNewerVersion(version, currentVersion), expected);
}

QTEST_GUILESS_MAIN(TestUpdateChecker)

#include "TestUpdateChecker.moc"
