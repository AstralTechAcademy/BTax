#include <QTest>
#include <tuple>
#include "ExchangeFactory.h"
#include "B2m.h"
#include "Importer.h"
#include "BrokerManager.h"
#include "Broker.h"
#include "SQLManager.h"
#include "Utils.h"
#include <QList>
#include <QUrl>

class ImporterTest: public QObject
{
Q_OBJECT

private slots:
    void initTestCase();
    void loadTestData();
    void detectExchange();
    void importB2m();
    void importBinance();
    void OperationsDuplicatedImporting();
    void cleanupTestCase();

private:
    std::shared_ptr<IExchange> b2m = ExchangeFactory::createExchange(EN_Exchange::B2M);
    UsersModel usersModel;
    OperationsModel operationsModel;
    WalletsModel walletsModel;
    WalletsModel walletsModelAll;
    WalletsPercModel walletsPercModel;
    CoinsModel coinsModel;
    AssetTypeModel assetTypeModel;
    NotificationManager notificationManager;

    BrokerManager* brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel);
    Importer *importer = Importer::getInstance(std::shared_ptr<BrokerManager>(brokerManager));

    QString pathB2m;
    QString pathB2m_1;
    QString pathB2mV2;
    QString pathB2mV2_1;    
    QString pathBinance;
    QString pathBinance_1;
};

void ImporterTest::initTestCase()
{
    QString version = "1.1.0";
    qDebug("Called before everything else.");
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(SQLManager::GetInstance()->getServer(), version, SQLManager::GetInstance()->getDatabase());
    engine.rootContext()->setContextProperty("BrokerImpl", broker);

    QCOMPARE(true, broker->openDatabase());
    
    pathB2m = QString("Files/B2M.csv");
    pathB2m_1 = QString("Files/B2M_1.csv");    
    pathB2mV2 = QString("Files/B2MV2.csv");
    pathB2mV2_1 = QString("Files/B2MV2_1.csv");     
    pathBinance = QString("Files/Binance.csv");
    pathBinance_1 = QString("Files/Binance_1.csv");
}

void ImporterTest::loadTestData()
{
    QSqlQuery query(SQLManager::getDb());

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
    QCOMPARE(true, SQLManager::GetInstance()->registerUser("user1"));
    QCOMPARE(true, SQLManager::GetInstance()->registerAsset("crypto", "ETH", "#FFFFF0")); // ID = 1
    QCOMPARE(true, SQLManager::GetInstance()->registerAsset("crypto", "ADA", "#FFFF00")); // ID = 2
    QCOMPARE(true, SQLManager::GetInstance()->registerAsset("crypto", "BTC", "#FFF000")); // ID = 3
    QCOMPARE(true, SQLManager::GetInstance()->registerAsset("fiat", "EUR", "#FF0000")); // ID = 4
    QCOMPARE(true, SQLManager::GetInstance()->registerAsset("fiat", "USD", "#FF0000")); // ID = 5
    QCOMPARE(true, SQLManager::GetInstance()->registerAsset("crypto", "LINK", "#FFF000")); // ID = 6
    QCOMPARE(true, SQLManager::GetInstance()->registerAsset("crypto", "B2M", "#FFF000")); // ID = 7
    QCOMPARE(true, SQLManager::GetInstance()->registerAsset("crypto", "SHIB", "#FFF000")); // ID = 8


    brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel);
    brokerManager->load();

}

void ImporterTest::detectExchange()
{
    EN_Exchange res = importer->detectExchange(pathB2m);
    QCOMPARE(EN_Exchange::B2M, res);
}

void ImporterTest::importB2m()
{
    importer->preview(EN_Exchange::B2M_V2, pathB2mV2);
    QCOMPARE(true, importer->opsAddedSize() == 4);
    
    importer->write();
    importer->preview(EN_Exchange::B2M_V2, pathB2mV2);
    QCOMPARE(true, importer->opsAlrdyAddedSize() == 4);

    // Newer file with more earn operations
    importer->preview(EN_Exchange::B2M_V2, pathB2mV2_1);
    QCOMPARE(true, importer->opsAddedSize() == 2);
    QCOMPARE(true, importer->opsAlrdyAddedSize() == 4);
    QCOMPARE(true, importer->opsWithErrorSize() == 0);
}

void ImporterTest::importBinance()
{
    importer->preview(EN_Exchange::BINANCE, pathBinance);
    QCOMPARE(true, importer->opsAddedSize() == 3);

    importer->write();
    importer->preview(EN_Exchange::BINANCE, pathBinance);  
    QCOMPARE(true, importer->opsAlrdyAddedSize() == 3);

    // Newer file with more earn operations
    importer->preview(EN_Exchange::BINANCE, pathBinance_1);    
    QCOMPARE(true, importer->opsAddedSize() == 2);
    QCOMPARE(true, importer->opsAlrdyAddedSize() == 4);
    QCOMPARE(true, importer->opsWithErrorSize() == 0);
}

void ImporterTest::OperationsDuplicatedImporting()
{
    QCOMPARE(true, false); // compare two values
}

void ImporterTest::cleanupTestCase()
{
    qDebug("Clean ImporterTest");
}

QTEST_GUILESS_MAIN(ImporterTest)

//Run specific testo run command: ./ImporterTest importBinance importB2m
#include "ImporterTest.moc"