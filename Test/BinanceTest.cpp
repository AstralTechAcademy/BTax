#include <QTest>
#include <tuple>
#include "ExchangeFactory.h"
#include "Binance.h"
#include "BrokerManager.h"
#include "Broker.h"
#include "SQLManager.h"
#include "Utils.h"
#include "Test_Utils.h"

class BinanceTest: public QObject
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
    std::shared_ptr<IExchange> exch = ExchangeFactory::createExchange(EN_Exchange::BINANCE);
    QList<std::shared_ptr<Operation>> operations;
    QString path = "Files/Binance.csv";

};

void BinanceTest::initTestCase()
{
    QString version = "1.1.0";
    qDebug("Called before everything else.");
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(version);
    engine.rootContext()->setContextProperty("BrokerImpl", broker);    
}

void BinanceTest::read()
{
    auto res = exch->read(path);
    QCOMPARE(true, res != std::nullopt); // compare two values
    operations = res.value();
    QCOMPARE(true, operations.size() > 0); // compare two values
}

void BinanceTest::parse()
{
    QCOMPARE(true, operations.size() > 0); // compare two values
    for (auto index = 0; index < operations.size(); index++)
    {
        auto op = operations[index];
        if(index == 0)
        {
            QCOMPARE(true, op->getPair2() == "ADA");
            QCOMPARE(true, op->getPair2Amount() == 0.13784104);
            QCOMPARE(true, QString::number(op->getPair2AmountFiat()) == "1.20154");
            QCOMPARE(true, QString::number(op->getGanancia()) == "0.165622"); // = "0.166666*1.19921" 
            QCOMPARE(true, op->getDate() == "31/12/2021 00:35:48");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "Binance") == "2021-12-31 00:35:48.000");
            //TODO: Date UTC+0
        }
        if(index == 1)
        {
            QCOMPARE(true, op->getPair2() == "AVAX");
            QCOMPARE(true, op->getPair2Amount() == 0.00006284);
            QCOMPARE(true, QString::number(op->getPair2AmountFiat()) == "89.9518");
            QCOMPARE(true, QString::number(op->getGanancia()) == "0.00565257"); // = 0.13784104*1.87601
            QCOMPARE(true, op->getDate() == "31/12/2021 00:39:24");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "Binance") == "2021-12-31 00:39:24.000");
        }     
        if(index == 2)
        {
            QCOMPARE(true, op->getPair2() == "DOT");
            QCOMPARE(true, op->getPair2Amount() == 0.00200880);
            QCOMPARE(true, QString::number(op->getPair2AmountFiat()) == "24.3487");
            QCOMPARE(true, QString::number(op->getGanancia()) == "0.0489117");
            QCOMPARE(true, op->getDate() == "31/12/2021 00:52:48");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "Binance") == "2021-12-31 00:52:48.000");
        }   
    }   
}

void BinanceTest::cleanupTestCase()
{
    qDebug("Clean BinanceTest");
}

QTEST_GUILESS_MAIN(BinanceTest)

//Run specific testo run command: ./BinanceTest read parse
#include "BinanceTest.moc"