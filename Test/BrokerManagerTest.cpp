#include <QTest>
#include <tuple>
#include "BrokerManager.h"
#include "Broker.h"
#include "SQLManager.h"

class BrokerManagerTest: public QObject
{
Q_OBJECT

private slots:
    void initTestCase();
    void loadTestData();
    void newAsset();
    void cleanupTestCase();
    void newOperation();
    void newSaleAssetMultiExchange();
    void newOperationWallet1NotExist();
    void newOperationWallet2NotExist();
    void newTransferencia();
    void newImportB2m();
    void newImportBinance();
    void newImportCrypto();
    void newDateCoversion();
    void newSale();
    void getAverage();
    void getGlobalAverage();
    void calculatePortfolio();



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

void BrokerManagerTest::initTestCase()
{
    QString version = "1.1.0";
    qDebug("Called before everything else.");
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(DBRemote::GetInstance()->getServer(), version, DBRemote::GetInstance()->getDatabase());
    engine.rootContext()->setContextProperty("BrokerImpl", broker);

    QCOMPARE(true, broker->openDatabase());
}

void BrokerManagerTest::loadTestData()
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
    QCOMPARE(true, DBLocal::GetInstance()->registerUser("user1"));
    QCOMPARE(true, DBLocal::GetInstance()->registerAsset("crypto", "ETH", "#FFFFF0")); // ID = 1
    QCOMPARE(true, DBLocal::GetInstance()->registerAsset("crypto", "ADA", "#FFFF00")); // ID = 2
    QCOMPARE(true, DBLocal::GetInstance()->registerAsset("crypto", "BTC", "#FFF000")); // ID = 3
    QCOMPARE(true, DBLocal::GetInstance()->registerAsset("fiat", "EUR", "#FF0000")); // ID = 4

    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("1", "Binance", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("1", "B2M", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("1", "Coinbase", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("1", "Crypto", 1));

    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("2", "Binance", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("2", "B2M", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("2", "Coinbase", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("2", "Crypto", 1));

    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("3", "Binance", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("3", "B2M", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("3", "Coinbase", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("3", "Crypto", 1));

    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("4", "Binance", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("4", "B2M", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("4", "Coinbase", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("4", "Crypto", 1));

    brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel);
    brokerManager->load();
    std::cout << "hh";

}

void BrokerManagerTest::newAsset()
{
    QCOMPARE(true, brokerManager->newAsset("stock", "prueba", "#FFFFFF")); // compare two values
}

void BrokerManagerTest::newOperation()
{
    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(true, brokerManager->newDeposit(13, 150.0, 0.0, "deposit 1", "")); // compare two values
    QCOMPARE(1, brokerManager->newOperation(13,1, 90.0, 1.0, 0.03, 3000.0, "EUR", 10.0, 1.0, "Compra 1", "Compra", "", "02/10/2022 10:00:00", wOpsModified)); // compare two values
    QCOMPARE(1, brokerManager->newOperation(13,1, 40.0, 1.0, 0.03, 1500.0, "EUR", 10.0, 1.0, "Compra 1", "Compra", "", "02/10/2022 10:00:01", wOpsModified)); // compare two values

    auto operation = brokerManager->getLastOperation();

    QCOMPARE(true, operation->getPair1() == "EUR");
    QCOMPARE(true, operation->getPair2() == "ETH");
    QCOMPARE(true, operation->getPair1Amount() == 40);
    QCOMPARE(true, operation->getPair2Amount() == 0.03);
    QCOMPARE(true, operation->getComision() == 10);
    QCOMPARE(true, operation->getDate() == "dom. oct. 2 10:00:01 2022");
}

void BrokerManagerTest::newSaleAssetMultiExchange()
{
    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(true, brokerManager->newDeposit(15, 133.0, 0.0, "deposit 2", "")); // compare two values
    QCOMPARE(true, brokerManager->newDeposit(14, 133.0, 0.0, "deposit 2", "")); // compare two values
    QCOMPARE(1, brokerManager->newOperation(15,5, 90.0, 1.0, 184.804928131, 0.487, "EUR", 10.0, 1.0, "newSaleAssetMultiExchange Compra 1", "Compra", "", "15/10/2022 13:00:00", wOpsModified)); // compare two values

    auto operation = brokerManager->getLastOperation();
    QCOMPARE(true, operation->getPair1() == "EUR");
    QCOMPARE(true, operation->getPair2() == "ADA");
    QCOMPARE(true, operation->getPair1Amount() == 90.0);
    QCOMPARE(true, operation->getPair2Amount() == 184.804928131);
    QCOMPARE(true, operation->getGanancia() == -10.0);
    QCOMPARE(true, operation->getDate() == "sáb. oct. 15 13:00:00 2022");

    QCOMPARE(1, brokerManager->newOperation(14,6, 100.0, 1.0, 147.492625369, 0.678, "EUR", 5.0, 1.0, "newSaleAssetMultiExchange Compra 2", "Compra", "", "15/10/2022 13:00:01", wOpsModified)); // compare two values
    QCOMPARE(1, brokerManager->newOperation(6,14, 58.0, 0.3489, 20.2362, 1.0, "EUR", 10.0, 0.3489, "newSaleAssetMultiExchange Venta 1", "Venta", "", "16/10/2022 16:00:01", wOpsModified)); // compare two values

    auto ganancia = (58.0 + 10.0) * (0.3489 - 0.487);
    operation = brokerManager->getLastOperation();
    QCOMPARE(true, operation->getPair1() == "ADA");
    QCOMPARE(true, operation->getPair2() == "EUR");
    QCOMPARE(true, operation->getPair1Amount() == 58.0);
    QCOMPARE(true, operation->getPair2Amount() == 20.2362);
    QCOMPARE(true, operation->getGanancia() == ganancia);
    QCOMPARE(true, operation->getDate() == "dom. oct. 16 16:00:01 2022");
}

void BrokerManagerTest::getAverage()
{
    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(true, brokerManager->newDeposit(1, 177.0, 0.0, "deposit getAverage", "")); // compare two values
    QCOMPARE(true, brokerManager->newDeposit(2, 177.0, 0.0, "deposit getAverage", "")); // compare two values
    QCOMPARE(1, brokerManager->newOperation(1,9, 56.0, 1.0, 0.00085826043, 65248.26, "EUR", 0.0, 1.0, "getAverage Compra 1", "Compra", "", "17/10/2022 13:00:00", wOpsModified)); // compare two values
    QCOMPARE(1, brokerManager->newOperation(1,9, 80.0, 1.0, 0.00189131555, 42298.6, "EUR", 0.0, 1.0, "getAverage Compra 2", "Compra", "", "17/10/2022 13:00:00", wOpsModified)); // compare two values
    QCOMPARE(1, brokerManager->newOperation(2,10, 90.0, 1.0, 0.00270690857, 33248.26, "EUR", 0.0, 1.0, "getAverage Compra 3", "Compra", "", "18/10/2022 13:00:00", wOpsModified)); // compare two values

    auto ws = DBLocal::GetInstance()->getCryptoWallets(BrokerManager::userID);

    for(auto w : ws.value())
    {
        auto average = 0.0;
        if(w->getWalletID() == 9)
        {
            average = (56.0+80.0)/(0.00085826043 + 0.00189131555);
            QCOMPARE(true, (int) w->getAverageCost() == (int) average);
        }

        if(w->getWalletID() == 10)
        {
            average = 33248.26;
            QCOMPARE(true, (int) w->getAverageCost() == (int) average);
        }
    }

    QCOMPARE(1, brokerManager->newOperation(9,1, 0.000143, 65000, 9.295, 1.0, "EUR", 0.0, 1.0, "getAverage Venta 1", "Venta", "", "17/10/2022 13:00:00", wOpsModified)); // compare two values

    ws = DBLocal::GetInstance()->getCryptoWallets(BrokerManager::userID);
    for(auto w : ws.value())
    {
        auto average = 0.0;
        if(w->getWalletID() == 9)
        {
            average = (46.6694+80.0)/(0.00071526043+0.00189131555);
            QCOMPARE(true, (int) w->getAverageCost() == (int) average);
        }
    }
}


void BrokerManagerTest::getGlobalAverage()
{

}

void BrokerManagerTest::calculatePortfolio()
{

}



void BrokerManagerTest::newTransferencia()
{
    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(1, brokerManager->newOperation(1,2, 0.04, 0.0, 0.04, 0.0, "ETH", 0.0, 1.0, "Transferencia 1", "Transferencia", "", "03/10/2022 10:00:00", wOpsModified)); // compare two values

    auto operations = brokerManager->getLastNWalletOperation(wOpsModified.size());
    auto indexModified = operations.size() - 1 ;
    for(auto index = 0; index <  operations.size(); index++)
    {
        QCOMPARE(true, operations[index]->getCoin() == "ETH");
        QCOMPARE(true, wOpsModified.size() == 2);
        QCOMPARE(true, operations[index]->getAmount() == wOpsModified[indexModified].getRetired());
        QCOMPARE(true, operations[index]->getDate().toString() == wOpsModified[indexModified].getDate().toString());
        indexModified--;
    }
}

void BrokerManagerTest::newSale()
{
    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(true, brokerManager->newDeposit(13, 150.0, 0.0, "deposit 1", "")); // compare two values
    QCOMPARE(1, brokerManager->newOperation(13,1, 90.0, 1.0, 0.03, 3000.0, "EUR", 10.0, 1.0, "Compra 1", "Compra", "", "02/10/2022 10:00:00", wOpsModified)); // compare two values
    QCOMPARE(1, brokerManager->newOperation(13,1, 40.0, 1.0, 0.03, 1500.0, "EUR", 10.0, 1.0, "Compra 1", "Compra", "", "02/10/2022 10:00:01", wOpsModified)); // compare two values

    //TODO: Venta

    /*auto operation = brokerManager->getLastOperation();

    QCOMPARE(true, operation->getPair1() == "EUR");
    QCOMPARE(true, operation->getPair2() == "ETH");
    QCOMPARE(true, operation->getPair1Amount() == 40);
    QCOMPARE(true, operation->getPair2Amount() == 0.03);
    QCOMPARE(true, operation->getComision() == 10);
    QCOMPARE(true, operation->getDate() == "dom. oct. 2 10:00:01 2022");*/
}

void BrokerManagerTest::newImportB2m()
{
 //TODO
}

void BrokerManagerTest::newImportBinance()
{
    //TODO
}

void BrokerManagerTest::newImportCrypto()
{
    //TODO
}

void BrokerManagerTest::newDateCoversion()
{
    //TODO
}

void BrokerManagerTest::newOperationWallet1NotExist()
{
    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(true, brokerManager->newDeposit(13, 100.0, 0.0, "deposit 1", "")); // compare two values
    QCOMPARE(3, brokerManager->newOperation(99,1, 90.0, 1.0, 0.03, 3000.0, "EUR", 10.0, 1.0, "Compra 1", "Compra", "", "02/10/2022 10:00:00", wOpsModified)); // compare two values
}

void BrokerManagerTest::newOperationWallet2NotExist()
{
    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(true, brokerManager->newDeposit(13, 100.0, 0.0, "deposit 1", "")); // compare two values
    QCOMPARE(4, brokerManager->newOperation(13,99, 90.0, 1.0, 0.03, 3000.0, "EUR", 10.0, 1.0, "Compra 1", "Compra", "", "02/10/2022 10:00:00", wOpsModified)); // compare two values
}

void BrokerManagerTest::cleanupTestCase()
{
    qDebug("Clean BrokerManagerTest");
}

QTEST_MAIN(BrokerManagerTest)
#include "BrokerManagerTest.moc"