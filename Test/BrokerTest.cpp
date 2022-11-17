#include <QTest>
#include "Broker.h"

class BrokerTest: public QObject
{
Q_OBJECT

private slots:
    void openDatabase();

private:
};

void BrokerTest::openDatabase()
{
    QString version = "1.1.0";
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(DBRemote::GetInstance()->getServer(), version, DBRemote::GetInstance()->getDatabase());
    engine.rootContext()->setContextProperty("BrokerImpl", broker);

    QCOMPARE(true, broker->openDatabase());
}


QTEST_MAIN(BrokerTest)
#include "BrokerTest.moc"