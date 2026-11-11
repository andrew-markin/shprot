#include <QCoreApplication>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>

#include "SingleInstance.h"

class TestSingleInstance : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void secondInstanceActivatesFirst();
    void quitMessageReachesFirst();
    void startsAgainAfterFirstQuits();
    void namesAreSeparate();

private:
    QString name(const QString& suffix = QString()) const;

private:
    QTemporaryDir* m_temporaryDir = nullptr;
};

void TestSingleInstance::init()
{
    m_temporaryDir = new QTemporaryDir();
    QVERIFY(m_temporaryDir->isValid());
}

void TestSingleInstance::cleanup()
{
    delete m_temporaryDir;
}

// Unique for the run, so tests running at once do not meet. The colon is there like in the real name.
QString TestSingleInstance::name(const QString& suffix) const
{
    return QString("ShprotTest:%1-%2%3").arg(QCoreApplication::applicationPid()).arg(qintptr(m_temporaryDir)).arg(suffix);
}

void TestSingleInstance::secondInstanceActivatesFirst()
{
    SingleInstance first(name(), m_temporaryDir->path());
    QSignalSpy activationSpy(&first, &SingleInstance::activationRequested);
    QSignalSpy quitSpy(&first, &SingleInstance::quitRequested);
    QVERIFY(first.start());

    SingleInstance second(name(), m_temporaryDir->path());
    QVERIFY(!second.start());

    QTRY_COMPARE(activationSpy.count(), 1);
    QCOMPARE(quitSpy.count(), 0);
}

void TestSingleInstance::quitMessageReachesFirst()
{
    SingleInstance first(name(), m_temporaryDir->path());
    QSignalSpy activationSpy(&first, &SingleInstance::activationRequested);
    QSignalSpy quitSpy(&first, &SingleInstance::quitRequested);
    QVERIFY(first.start());

    QVERIFY(SingleInstance::sendMessage(name(), "quit"));
    QVERIFY(SingleInstance::sendMessage(name(), "unknown"));

    QTRY_COMPARE(quitSpy.count(), 1);
    QCOMPARE(activationSpy.count(), 0);
}

void TestSingleInstance::startsAgainAfterFirstQuits()
{
    {
        SingleInstance first(name(), m_temporaryDir->path());
        QVERIFY(first.start());
    }

    SingleInstance again(name(), m_temporaryDir->path());
    QVERIFY(again.start());
}

void TestSingleInstance::namesAreSeparate()
{
    SingleInstance first(name("a"), m_temporaryDir->path());
    SingleInstance other(name("b"), m_temporaryDir->path());

    QVERIFY(first.start());
    QVERIFY(other.start());
}

QTEST_GUILESS_MAIN(TestSingleInstance)
#include "TestSingleInstance.moc"
