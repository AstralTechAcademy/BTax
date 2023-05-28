#include <QTest>
#include <tuple>
#include "BrokerManager.h"
#include "Broker.h"
#include "SQLManager.h"
#include "Utils.h"
#include "Test_Utils.h"

class SQLManagerTest: public QObject
{
Q_OBJECT

private slots:
    void openDatabase_ok(void);
    void getVersion_0(void);
    void update(void);
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
    void dropTables(void);    
};

void SQLManagerTest::openDatabase_ok(void)
{
    QString version = "1.1.0";
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(version);
    engine.rootContext()->setContextProperty("BrokerImpl", broker);
    QCOMPARE(true, SQLManager::GetInstance()->openDatabase());
}

void SQLManagerTest::dropTables(void)
{
    QSqlQuery query(SQLManager::getDb());
    query.prepare("drop table  WalletOperations, Operations, Withdraws, Deposits, Wallets, Coins, Version, Exchanges, AssetType, Users");
    query.exec();
}

void SQLManagerTest::getVersion_0(void)
{
    dropTables();
    QCOMPARE(true, SQLManager::GetInstance()->getVersion() == 0);
}

void SQLManagerTest::update(void)
{
    QCOMPARE(true, SQLManager::GetInstance()->update());
    QCOMPARE(true, SQLManager::GetInstance()->getVersion() == 202000);
}

void SQLManagerTest::cleanupTestCase()
{
    qDebug("Clean SQLManagerTest");
}

QTEST_MAIN(SQLManagerTest)
#include "SQLManagerTest.moc"