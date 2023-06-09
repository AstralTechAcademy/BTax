#include <QTest>
#include <tuple>
#include "ExchangeFactory.h"
#include "B2mV2.h"
#include "BrokerManager.h"
#include "Broker.h"
#include "SQLManager.h"
#include "Utils.h"
#include "Test_Utils.h"

class B2mV2Test: public QObject
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
    std::shared_ptr<IExchange> b2m = ExchangeFactory::createExchange(EN_Exchange::B2M_V2);
    QList<std::shared_ptr<Operation>> operations;
    QString pathB2m = "Files/B2MV2.csv";

};

void B2mV2Test::initTestCase()
{
    QString version = "1.1.0";
    qDebug("Called before everything else.");
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(version);
    engine.rootContext()->setContextProperty("BrokerImpl", broker);
}

void B2mV2Test::read()
{
    auto res = b2m->read(pathB2m);
    QCOMPARE(true, res != std::nullopt); // compare two values
    operations = res.value();
    QCOMPARE(true, operations.size() > 0); // compare two values
}

void B2mV2Test::parse()
{
    QCOMPARE(true, operations.size() > 0); // compare two values
    for (auto index = 0; index < operations.size(); index++)
    {
        auto op = operations[index];
        if(index == 0)
        {
            QCOMPARE(true, op->getPair2() == "B2M");
            QCOMPARE(true, op->getPair2Amount() == 11.0089438);
            QCOMPARE(true, QString::number(op->getPair2AmountFiat()) == "0.0584357");
            QCOMPARE(true, QString::number(op->getGanancia()) == "0.643316");
            QCOMPARE(true, op->getDate() == "01/01/2022 12:05:00");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "B2M") == "2022-01-01 11:05:00.000");
            //TODO: Date UTC+0
        }
        if(index == 1)
        {
            QCOMPARE(true, op->getPair2() == "B2M");
            QCOMPARE(true, op->getPair2Amount() == 7.24459346);
            QCOMPARE(true, QString::number(op->getPair2AmountFiat()) == "0.0638296");
            QCOMPARE(true, QString::number(op->getGanancia()) == "0.462419");
            QCOMPARE(true, op->getDate() == "02/01/2022 12:01:00");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "B2M") == "2022-01-02 11:01:00.000");
        }
        if(index == 2)
        {
            QCOMPARE(true, op->getPair2() == "B2M");
            QCOMPARE(true, op->getPair2Amount() == 6.9315493);
            QCOMPARE(true, QString::number(op->getPair2AmountFiat()) == "0.0613843");
            QCOMPARE(true, QString::number(op->getGanancia()) == "0.425488");
            QCOMPARE(true, op->getDate() == "03/01/2022 12:10:00");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "B2M") == "2022-01-03 11:10:00.000");
        }   
        if(index == 3)
        {
            QCOMPARE(true, op->getPair2() == "B2M");
            QCOMPARE(true, op->getPair2Amount() == 6.71889541);
            QCOMPARE(true, QString::number(op->getPair2AmountFiat()) == "0.060033");
            QCOMPARE(true, QString::number(op->getGanancia()) == "0.403355");
            QCOMPARE(true, op->getDate() == "04/01/2022 12:01:00");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "B2M") == "2022-01-04 11:01:00.000");
        }         
    }   
}

void B2mV2Test::cleanupTestCase()
{
    qDebug("Clean B2mTest");
}

QTEST_GUILESS_MAIN(B2mV2Test)

//Run specific testo run command: ./B2mV2Test read parse
#include "B2mV2Test.moc"