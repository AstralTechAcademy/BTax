#include <QTest>
#include <tuple>
#include "ExchangeFactory.h"
#include "B2m.h"
#include "BrokerManager.h"
#include "Broker.h"
#include "SQLManager.h"
#include "Utils.h"
#include "Test_Utils.h"
class B2mTest: public QObject
{
Q_OBJECT

private slots:
    void initTestCase();
    void read();
    void parse();
    void cleanupTestCase();

private:
    UsersModel usersModel;
    OperationsModel operationsModel;
    WalletsModel walletsModel;
    WalletsModel walletsModelAll;
    WalletsPercModel walletsPercModel;
    CoinsModel coinsModel;
    ExchangesModel exchangesModel;
    AssetTypeModel assetTypeModel;
    NotificationManager* notificationManager = NotificationManager::getInstance();
    BrokerManager* brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel, &exchangesModel);
    std::shared_ptr<IExchange> b2m = ExchangeFactory::createExchange(EN_Exchange::B2M);
    QList<std::shared_ptr<Operation>> operations;
    QString pathB2m = "Files/B2M.csv";

};

void B2mTest::initTestCase()
{
    QString version = "1.1.0";
    qDebug("Called before everything else.");
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(version);
    engine.rootContext()->setContextProperty("BrokerImpl", broker);
}

void B2mTest::read()
{
    auto res = b2m->read(pathB2m);
    QCOMPARE(true, res != std::nullopt); // compare two values
    operations = res.value();
    QCOMPARE(true, operations.size() > 0); // compare two values
}

void B2mTest::parse()
{
    QCOMPARE(true, operations.size() > 0); // compare two values
    for (auto index = 0; index < operations.size(); index++)
    {
        auto op = operations[index];
        if(index == 0)
        {
            QCOMPARE(true, op->getPair2() == "B2M");
            QCOMPARE(true, op->getPair2Amount() == 10.95043612);
            QCOMPARE(true, op->getPair2AmountFiat() == 0.079312212);
            QCOMPARE(true, op->getGanancia() == 10.95043612*0.079312212);
            qDebug() << op->getDate() << "  " << op->getDateTime();
            QCOMPARE(true, op->getDate() == "30/12/2021 12:15:12");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "B2M") == "2021-12-30 11:15:12.000");
            //TODO: Date UTC+0
        }
        if(index == 1)
        {
            QCOMPARE(true, op->getPair2() == "B2M");
            QCOMPARE(true, op->getPair2Amount() == 10.13447556);
            QCOMPARE(true, op->getPair2AmountFiat() == 0.06558892405595308);
            QCOMPARE(true, op->getGanancia() == 10.13447556*0.06558892405595308);
                        qDebug() << op->getDate() << "  " << op->getDateTime();

            QCOMPARE(true, op->getDate() == "31/12/2021 15:15:03");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "B2M") == "2021-12-31 14:15:03.000");
        }       
    }   
}

void B2mTest::cleanupTestCase()
{
    qDebug("Clean B2mTest");
}

QTEST_MAIN(B2mTest)
#include "B2mTest.moc"