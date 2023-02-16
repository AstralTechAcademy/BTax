#include <QTest>
#include "Broker.h"

class BrokerTest: public QObject
{
Q_OBJECT

private slots:
    void openDatabase();
    void prepareScenario();
    void noUsersInDatabase();
    void userAvailable();    

private:
    UsersModel usersModel;
    OperationsModel operationsModel;
    WalletsModel walletsModel;
    WalletsModel walletsModelAll;
    WalletsPercModel walletsPercModel;
    CoinsModel coinsModel;
    AssetTypeModel assetTypeModel;
    ExchangesModel exchangesModel;
    NotificationManager notificationManager;
    BrokerManager* brokerManager;
};

void BrokerTest::prepareScenario()
{
    QSqlQuery query(SQLManager::getDb());
    query.prepare("drop table  WalletOperations, Operations, Withdraws, Deposits, Wallets, Coins, Version, Exchanges, AssetType, Users");
    query.exec();

    SQLManager::GetInstance()->init();
}

void BrokerTest::openDatabase()
{
    QString version = "1.1.0";
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(SQLManager::GetInstance()->getServer(), version, SQLManager::GetInstance()->getDatabase());
    engine.rootContext()->setContextProperty("BrokerImpl", broker);

    QCOMPARE(true, broker->openDatabase());
}

void BrokerTest::noUsersInDatabase()
{
    QString version = "1.1.0";
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(SQLManager::GetInstance()->getServer(), version, SQLManager::GetInstance()->getDatabase());
    engine.rootContext()->setContextProperty("BrokerImpl", broker);

    QCOMPARE(BrokerManager::LoadResCode::NO_USERS, broker->load());
}

void BrokerTest::userAvailable()
{
    QString version = "1.1.0";
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(SQLManager::GetInstance()->getServer(), version, SQLManager::GetInstance()->getDatabase());
    engine.rootContext()->setContextProperty("BrokerImpl", broker);
    SQLManager::GetInstance()->registerUser("user1");
    SQLManager::GetInstance()->registerAsset("fiat", "EUR", "#FF0000");

    brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel, &exchangesModel);
    brokerManager->load();    

    QCOMPARE(BrokerManager::LoadResCode::OK, broker->load());
}


QTEST_MAIN(BrokerTest)
#include "BrokerTest.moc"