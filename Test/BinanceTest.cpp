#include <QTest>
#include <tuple>
#include "ExchangeFactory.h"
#include "Binance.h"
#include "BrokerManager.h"
#include "Broker.h"
#include "SQLManager.h"
#include "Utils.h"

class BinanceTest: public QObject
{
Q_OBJECT

private slots:
    void initTestCase();
    void read();
    void parse();
    void cleanupTestCase();

private:
    std::shared_ptr<IExchange> exch = ExchangeFactory::createExchange(EN_Exchange::BINANCE);
    QList<std::shared_ptr<Operation>> operations;
    QString path = "Files/Binance.csv";

};

void BinanceTest::initTestCase()
{
    QString version = "1.1.0";
    qDebug("Called before everything else.");
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
            QCOMPARE(true, op->getPair2Amount() == 0.166666);
            QCOMPARE(true, op->getPair2AmountFiat() == 0.079312212);
            QCOMPARE(true, op->getGanancia() == 0.166666*0.079312212);
            qDebug() << op->getDate() << "  " << op->getDateTime();
            QCOMPARE(true, op->getDate() == "30/12/2021 00:35:48");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "Binance") == "2021-12-30 00:35:48.000");
            //TODO: Date UTC+0
        }
        if(index == 1)
        {
            QCOMPARE(true, op->getPair2() == "ADA");
            QCOMPARE(true, op->getPair2Amount() == 0.13784104);
            QCOMPARE(true, op->getPair2AmountFiat() == 0.06558892405595308);
            QCOMPARE(true, op->getGanancia() == 0.13784104*0.06558892405595308);
            qDebug() << op->getDate() << "  " << op->getDateTime();

            QCOMPARE(true, op->getDate() == "15/10/2021 01:44:48");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "Binance") == "2021-10-15 01:44:48.000");
        }     
        if(index == 2)
        {
            QCOMPARE(true, op->getPair2() == "SHIB");
            QCOMPARE(true, op->getPair2Amount() == 4516.28000000);
            QCOMPARE(true, op->getPair2AmountFiat() == 0.06558892405595308);
            QCOMPARE(true, op->getGanancia() == 4516.28000000*0.06558892405595308);
            qDebug() << op->getDate() << "  " << op->getDateTime();

            QCOMPARE(true, op->getDate() == "14/10/2021 2:19:9");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "Binance") == "2021-10-14 02:19:09.000");
        }   
    }   
}

void BinanceTest::cleanupTestCase()
{
    qDebug("Clean BinanceTest");
}

QTEST_MAIN(BinanceTest)
#include "BinanceTest.moc"