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
            QCOMPARE(true, QString::number(op->getPair2AmountFiat()) == "1.19921");
            QCOMPARE(true, QString::number(op->getGanancia()) == "0.199868"); // = "0.166666*1.19921" 
            qDebug() << op->getDate();
            QCOMPARE(true, op->getDate() == "31/12/2021 00:35:48");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "Binance") == "2021-12-31 00:35:48.000");
            //TODO: Date UTC+0
        }
        if(index == 1)
        {
            QCOMPARE(true, op->getPair2() == "ADA");
            QCOMPARE(true, op->getPair2Amount() == 0.13784104);
            QCOMPARE(true, QString::number(op->getPair2AmountFiat()) == "1.87601");
            QCOMPARE(true, QString::number(op->getGanancia()) == "0.258591"); // = 0.13784104*1.87601
            QCOMPARE(true, op->getDate() == "15/10/2021 01:44:48");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "Binance") == "2021-10-15 01:44:48.000");
        }     
        if(index == 2)
        {
            QCOMPARE(true, op->getPair2() == "SHIB");
            QCOMPARE(true, op->getPair2Amount() == 4516.28000000);
            qDebug() <<   QString::number(op->getPair2AmountFiat()) <<" "<< QString::number(op->getGanancia());
            QCOMPARE(true, QString::number(op->getPair2AmountFiat()) == "2.50331e-05");
            QCOMPARE(true, QString::number(op->getGanancia()) == "0.113056");
            QCOMPARE(true, op->getDate() == "14/10/2021 02:19:09");
            QCOMPARE(true, dateTimeToUTC0(op->getDateTime(), "Binance") == "2021-10-14 02:19:09.000");
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