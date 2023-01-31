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
    NotificationManager notificationManager;
    BrokerManager* brokerManager;
};

void BrokerTest::prepareScenario()
{
    QSqlQuery query(DBLocal::getDb());

    query.prepare("drop table WalletOperations, Operations, Deposits, Wallets, Users, Coins");
    query.exec();

    query.prepare("create or replace table Coins("
                  "    id    int auto_increment primary key,"
                  "    name  varchar(10)                  null,"
                  "    type  varchar(10) default 'crypto' null,"
                  "    color varchar(10)                  null);");
    query.exec();
    query.prepare("create or replace table Users("
                  " id    int auto_increment primary key,"
                  " username varchar(20) null);");
    query.exec();
    query.prepare("create or replace table Wallets("
                  " id    int auto_increment primary key,"
                  " coin     int           null,"
                  " exchange varchar(50)   null,"
                  " user     int default 1 null,"
                  " pubkey   varchar(256)  null,"
                  " constraint Wallets_ibfk_1"
                  " foreign key (user) references Users (id),"
                  " constraint Wallets_ibfk_2"
                  " foreign key (coin) references Coins (id));");
    query.exec();
    query.prepare("create or replace table Deposits("
                  " id    int auto_increment primary key,"
                  " wallet int              null,"
                  " amount double default 0 null,"
                  " fees   double default 0 null,"
                  " date   varchar(50)      null,"
                  " constraint Deposits_ibfk_1"
                  " foreign key (wallet) references Wallets (id));");
    query.exec();

    query.prepare("create or replace table Operations("
                  " id    int auto_increment primary key,"
                  "    wallet1         int                                 null,"
                  "    wallet2         int                                 null,"
                  "    pair1Amount     double      default 0               null,"
                  "    pair1AmountFiat double      default 0               null,"
                  "    pair2Amount     double      default 0               null,"
                  "    pair2AmountFiat double      default 0               null,"
                  "    comision        double      default 0               null,"
                  "    comisionFiat    double      default 0               null,"
                  "    ganancia        double      default 0               null,"
                  "    status          varchar(50) default 'Not Confirmed' null,"
                  "    date            varchar(50)                         null,"
                  "    comments        varchar(512)                        null,"
                  "    type            varchar(20)                         null,"
                  "    constraint Operations_ibfk_1"
                  "        foreign key (wallet1) references Wallets (id),"
                  "    constraint Operations_ibfk_2"
                  "        foreign key (wallet2) references Wallets (id));");
    query.exec();

    query.prepare("create or replace table WalletOperations("
                  " id    int auto_increment primary key,"
                  "    wallet      int              null,"
                  "    amount      double default 0 null,"
                  "    retired     double default 0 null,"
                  "    available   double default 0 null,"
                  "    fiat        double default 0 null,"
                  "    date        varchar(50)      null,"
                  "    datetimeUTC datetime         null,"
                  "    available3  double as (`amount` - `retired`) stored,"
                  "    constraint WalletOperations_ibfk_1"
                  "        foreign key (wallet) references Wallets (id));");
    query.exec();
}

void BrokerTest::openDatabase()
{
    QString version = "1.1.0";
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(DBRemote::GetInstance()->getServer(), version, DBRemote::GetInstance()->getDatabase());
    engine.rootContext()->setContextProperty("BrokerImpl", broker);

    QCOMPARE(true, broker->openDatabase());
}

void BrokerTest::noUsersInDatabase()
{
    QString version = "1.1.0";
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(DBRemote::GetInstance()->getServer(), version, DBRemote::GetInstance()->getDatabase());
    engine.rootContext()->setContextProperty("BrokerImpl", broker);

    QCOMPARE(BrokerManager::LoadResCode::NO_USERS, broker->load());
}

void BrokerTest::userAvailable()
{
    QString version = "1.1.0";
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(DBRemote::GetInstance()->getServer(), version, DBRemote::GetInstance()->getDatabase());
    engine.rootContext()->setContextProperty("BrokerImpl", broker);
    DBLocal::GetInstance()->registerUser("user1");
    DBLocal::GetInstance()->registerAsset("fiat", "EUR", "#FF0000");

    brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel);
    brokerManager->load();    

    QCOMPARE(BrokerManager::LoadResCode::OK, broker->load());
}


QTEST_MAIN(BrokerTest)
#include "BrokerTest.moc"