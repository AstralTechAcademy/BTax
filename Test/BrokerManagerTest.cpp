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
    void newDateCoversion();
    void newSale();
    void getAverage();
    void getGlobalAverage();
    void calculatePortfolio();
    void updateTimeUTC();

    void fullUseCase();



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
    QCOMPARE(true, DBLocal::GetInstance()->registerAsset("fiat", "USD", "#FF0000")); // ID = 5
    QCOMPARE(true, DBLocal::GetInstance()->registerAsset("crypto", "LINK", "#FFF000")); // ID = 6

    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("1", "Binance", 1)); // ID = 1
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("1", "B2M", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("1", "Coinbase", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("1", "Crypto", 1));

    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("2", "Binance", 1)); // ID = 5
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("2", "B2M", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("2", "Coinbase", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("2", "Crypto", 1));

    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("3", "Binance", 1)); // ID = 9
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("3", "B2M", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("3", "Coinbase", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("3", "Crypto", 1));

    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("4", "Binance", 1)); // ID = 13
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("4", "B2M", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("4", "Coinbase", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("4", "Crypto", 1));
    
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("5", "Binance", 1)); // ID = 17
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("5", "B2M", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("5", "Coinbase", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("5", "Bitpanda", 1)); // ID = 20

    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("6", "Binance", 1)); // ID = 21
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("6", "B2M", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("6", "Coinbase", 1));
    QCOMPARE(true, (bool) DBLocal::GetInstance()->addWallet("6", "Bitpanda", 1)); // ID = 24

    brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel);
    brokerManager->load();

}

void BrokerManagerTest::newAsset()
{
    QCOMPARE(true, brokerManager->newAsset("stock", "prueba", "#FFFFFF")); // compare two values
}

void BrokerManagerTest::newOperation()
{
    std::vector<WalletOperation> wOpsModified;
    WalletOperation::OperationData data;

    QCOMPARE(true, brokerManager->newDeposit(13, 150.0, 0.0, "deposit 1", "")); // compare two values
    
    data.walletID1 = 13;
    data.walletID2 = 1;
    data.pair1Amount = 90.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 0.03;
    data.pair2AmountFiat = 3000.0;
    data.feesCoin = "EUR";
    data.comision = 10.0;
    data.comisionFiat = 1.0;
    data.comments = "Compra 1";
    data.type = "Compra";
    data.status = "";
    data.date = "02/10/2022 10:00:00";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values
        
    data.walletID1 = 13;
    data.walletID2 = 1;
    data.pair1Amount = 40.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 0.03;
    data.pair2AmountFiat = 1500.0;
    data.feesCoin = "EUR";
    data.comision = 10.0;
    data.comisionFiat = 1.0;
    data.comments = "Compra 1";
    data.type = "Compra";
    data.status = "";
    data.date = "02/10/2022 10:00:01";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values



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
    WalletOperation::OperationData data;
    std::vector<const WalletOperation*> ws;
    std::optional<std::vector<const WalletOperation*>> wallet;

    QCOMPARE(true, brokerManager->newDeposit(15, 133.0, 0.0, "deposit 2", "")); // compare two values
    QCOMPARE(true, brokerManager->newDeposit(14, 133.0, 0.0, "deposit 2", "")); // compare two values

    data.walletID1 = 15;
    data.walletID2 = 5;
    data.pair1Amount = 90.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 184.804928131;
    data.pair2AmountFiat = 0.487;
    data.feesCoin = "EUR";
    data.comision = 10.0;
    data.comisionFiat = 1.0;
    data.comments = "newSaleAssetMultiExchange Compra 1";
    data.type = "Compra";
    data.status = "";
    data.date = "15/10/2022 13:00:00";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values


    auto operation = brokerManager->getLastOperation();
    QCOMPARE(true, operation->getPair1() == "EUR");
    QCOMPARE(true, operation->getPair2() == "ADA");
    QCOMPARE(true, operation->getPair1Amount() == 90.0);
    QCOMPARE(true, operation->getPair2Amount() == 184.804928131);
    QCOMPARE(true, operation->getGanancia() == -10.0);
    QCOMPARE(true, operation->getDate() == "sáb. oct. 15 13:00:00 2022");

    data.walletID1 = 14;
    data.walletID2 = 6;
    data.pair1Amount = 100.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 147.492625369;
    data.pair2AmountFiat = 0.678;
    data.feesCoin = "EUR";
    data.comision = 5.0;
    data.comisionFiat = 1.0;
    data.comments = "newSaleAssetMultiExchange Compra 2";
    data.type = "Compra";
    data.status = "";
    data.date = "15/10/2022 13:00:01";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values    
        
    data.walletID1 = 6;
    data.walletID2 = 14;
    data.pair1Amount = 58.0;
    data.pair1AmountFiat = 0.3489;
    data.pair2Amount = 20.2362;
    data.pair2AmountFiat = 1.0;
    data.feesCoin = "EUR";
    data.comision = 10.0;
    data.comisionFiat = 0.3489;
    data.comments = "newSaleAssetMultiExchange Venta 1";
    data.type = "Venta";
    data.status = "";
    data.date = "16/10/2022 16:00:01";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values

    auto ganancia = (58.0 + 10.0) * (0.3489 - 0.487);
    operation = brokerManager->getLastOperation();
    QCOMPARE(true, operation->getPair1() == "ADA");
    QCOMPARE(true, operation->getPair2() == "EUR");
    QCOMPARE(true, operation->getPair1Amount() == 58.0);
    QCOMPARE(true, operation->getPair2Amount() == 20.2362);
    std::cout << operation->getGanancia() << " " << ganancia;
    QCOMPARE(true, operation->getGanancia() == ganancia);
    QCOMPARE(true, operation->getDate() == "dom. oct. 16 16:00:01 2022");

    // Revisar la cantidad de monedas en la wallet ADA-Binance (tiene las monedas mas antiguas -> las primeras que se venden)
    QCOMPARE(true, wOpsModified.size() == 1);
    QCOMPARE(true, QString::number(wOpsModified[0].getRetired()) == "68");
    QCOMPARE(true, QString::number(wOpsModified[0].getAvailable()) == "116.805");

    /*******************************************************************************/
    /*******************************************************************************/
    /******************************** Segunda Venta ********************************/
    /*******************************************************************************/
    /*******************************************************************************/
    data.walletID1 = 6;
    data.walletID2 = 14;
    data.pair1Amount = 20.0;
    data.pair1AmountFiat = 0.3489;
    data.pair2Amount = 6.978;
    data.pair2AmountFiat = 1.0;
    data.feesCoin = "EUR";
    data.comision = 0.0;
    data.comisionFiat = 0.0;
    data.comments = "newSaleAssetMultiExchange Venta 2";
    data.type = "Venta";
    data.status = "";
    data.date = "16/10/2022 17:00:01";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values

    ganancia = (20.0 + 0.0) * (0.3489 - 0.487);
    operation = brokerManager->getLastOperation();
    QCOMPARE(true, operation->getPair1() == "ADA");
    QCOMPARE(true, operation->getPair2() == "EUR");
    QCOMPARE(true, operation->getPair1Amount() == 20.0);
    QCOMPARE(true, operation->getPair2Amount() == 6.978);
    QCOMPARE(true, operation->getGanancia() == ganancia);
    QCOMPARE(true, operation->getDate() == "dom. oct. 16 17:00:01 2022");

    // Revisar la cantidad de monedas en la wallet ADA-Binance (tiene las monedas mas antiguas -> las primeras que se venden)
    QCOMPARE(true, wOpsModified.size() == 1);
    QCOMPARE(true, QString::number(wOpsModified[0].getRetired()) == "88");
    QCOMPARE(true, QString::number(wOpsModified[0].getAvailable()) == "96.8049");

    // Revisar la cantidad de monedas en la wallet ADA-B2M
    wallet = DBLocal::GetInstance()->getWalletOperations("6");
    ws = wallet.value();
    QCOMPARE(true, ws.size() == 1);
    QCOMPARE(true, QString::number(ws[0]->getRetired()) == "0");
    QCOMPARE(true, QString::number(ws[0]->getAvailable()) == "147.493");

    // Revisar la cantidad de monedas en la wallet EUR-B2M
    wallet = DBLocal::GetInstance()->getWalletOperations("14");
    ws = wallet.value();
    QCOMPARE(true, ws.size() == 3);
    QCOMPARE(true, QString::number(ws[1]->getRetired()) == "0");
    QCOMPARE(true, QString::number(ws[1]->getAvailable()) == "20.2362");
    QCOMPARE(true, QString::number(ws[2]->getRetired()) == "0");
    QCOMPARE(true, QString::number(ws[2]->getAvailable()) == "6.978");

}

void BrokerManagerTest::getAverage()
{
    std::vector<WalletOperation> wOpsModified;
    WalletOperation::OperationData data;

    QCOMPARE(true, brokerManager->newDeposit(1, 177.0, 0.0, "deposit getAverage", "")); // compare two values
    QCOMPARE(true, brokerManager->newDeposit(2, 177.0, 0.0, "deposit getAverage", "")); // compare two values
    data.walletID1 = 1;
    data.walletID2 = 9;
    data.pair1Amount = 56.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 0.00085826043;
    data.pair2AmountFiat = 65248.26;
    data.feesCoin = "EUR";
    data.comision = 0.0;
    data.comisionFiat = 1.0;
    data.comments = "getAverage Compra 1";
    data.type = "Compra";
    data.status = "";
    data.date = "17/10/2022 13:00:00";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values
    
    data.walletID1 = 1;
    data.walletID2 = 9;
    data.pair1Amount = 80.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 0.00189131555;
    data.pair2AmountFiat = 42298.26;
    data.feesCoin = "EUR";
    data.comision = 0.0;
    data.comisionFiat = 1.0;
    data.comments = "getAverage Compra 2";
    data.type = "Compra";
    data.status = "";
    data.date = "17/10/2022 13:00:00";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values
    
    data.walletID1 = 2;
    data.walletID2 = 10;
    data.pair1Amount = 90.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 0.00270690857;
    data.pair2AmountFiat = 33248.26;
    data.feesCoin = "EUR";
    data.comision = 0.0;
    data.comisionFiat = 1.0;
    data.comments = "getAverage Compra 3";
    data.type = "Compra";
    data.status = "";
    data.date = "18/10/2022 13:00:00";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values

    auto ws = DBLocal::GetInstance()->getCryptoWallets(BrokerManager::userID);

    for(auto w : ws.value())
    {
        auto average = 0.0;
        if(w->getWalletID() == 9)
        {
            average = (56.0+80.0)/(0.0008582604 + 0.0018913155);
            QCOMPARE(true, (int) w->getAverageCost() == (int) average); // Este test no funciona porque los decimales no son precisos
        }

        if(w->getWalletID() == 10)
        {
            average = 33248.26;
            QCOMPARE(true, (int) w->getAverageCost() == (int) average);
        }
    }

    data.walletID1 = 9;
    data.walletID2 = 1;
    data.pair1Amount = 0.000143;
    data.pair1AmountFiat = 65000;
    data.pair2Amount = 9.295;
    data.pair2AmountFiat = 1.0;
    data.feesCoin = "EUR";
    data.comision = 0.0;
    data.comisionFiat = 1.0;
    data.comments = "getAverage Venta 1";
    data.type = "Venta";
    data.status = "";
    data.date = "19/10/2022 13:00:00";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values

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
    WalletOperation::OperationData data;

    data.walletID1 = 1;
    data.walletID2 = 2;
    data.pair1Amount = 0.04;
    data.pair1AmountFiat = 0.0;
    data.pair2Amount = 0.04;
    data.pair2AmountFiat = 0.0;
    data.feesCoin = "ETH";
    data.comision = 0.0;
    data.comisionFiat = 1.0;
    data.comments = "Transferencia 1";
    data.type = "Transferencia";
    data.status = "";
    data.date = "03/10/2022 10:00:00";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values

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
    WalletOperation::OperationData data;
    data.walletID1 = 13;
    data.walletID2 = 1;
    data.pair1Amount = 90.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 0.03;
    data.pair2AmountFiat = 3000.0;
    data.feesCoin = "EUR";
    data.comision = 10.0;
    data.comisionFiat = 1.0;
    data.comments = "Compra 1";
    data.type = "Compra";
    data.status = "";
    data.date = "02/10/2022 10:00:00";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values
        
    data.walletID1 = 13;
    data.walletID2 = 1;
    data.pair1Amount = 40.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 0.03;
    data.pair2AmountFiat = 1500.0;
    data.feesCoin = "EUR";
    data.comision = 10.0;
    data.comisionFiat = 1.0;
    data.comments = "Compra 1";
    data.type = "Compra";
    data.status = "";
    data.date = "02/10/2022 10:00:01";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values
    
    //TODO: Venta

    /*auto operation = brokerManager->getLastOperation();

    QCOMPARE(true, operation->getPair1() == "EUR");
    QCOMPARE(true, operation->getPair2() == "ETH");
    QCOMPARE(true, operation->getPair1Amount() == 40);
    QCOMPARE(true, operation->getPair2Amount() == 0.03);
    QCOMPARE(true, operation->getComision() == 10);
    QCOMPARE(true, operation->getDate() == "dom. oct. 2 10:00:01 2022");*/
}

/*
 * Description: Deposit 133 USD, Buy LINK, Sell Link (lossing), Sell Link (profit)
*/
void BrokerManagerTest::fullUseCase()
{
    // TEST WORKING. RESULT = OK

    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(true, brokerManager->newDeposit(20, 133.33333, 0.0, "deposit full use case", "")); // compare two values

    WalletOperation::OperationData data;
    data.walletID1 = 20;
    data.walletID2 = 24;
    data.pair1Amount = 68.6685;
    data.pair1AmountFiat = 1.0; // USD = EUR
    data.pair2Amount = 11.004567;
    data.pair2AmountFiat = 6.24;
    data.feesCoin = "USD";
    data.comision = 5.0;
    data.comisionFiat = 1.0; // USD = EUR
    data.comments = "Compra Full Use case";
    data.type = "Compra";
    data.status = "";
    data.date = "30/12/2021 23:59:59";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values

    // Check retired amount
    auto wops = brokerManager->getLastNWalletOperation(2);
    QCOMPARE(true, wops.at(0)->getCoin() == "LINK");
    QCOMPARE(true, wops.at(0)->getAmount() == 11.004567);
    QCOMPARE(true, wops.at(0)->getAvailable() == 11.004567);
    QCOMPARE(true, wops.at(0)->getRetired() == 0);
    QCOMPARE(true, wops.at(0)->getFiatPrice() == 6.24);

    QCOMPARE(true, wops.at(1)->getCoin() == "USD");
    QCOMPARE(true, wops.at(1)->getRetired() == 73.6685);
    QCOMPARE(true, wops.at(1)->getAvailable() == 59.664829999999995);

    auto w = std::get<1>(DBLocal::GetInstance()->getWallet(24));
    QCOMPARE(true, QString::number(w->getAverageCost()) == "6.24");

    data.walletID1 = 24;
    data.walletID2 = 20;
    data.pair1Amount = 4.9632;
    data.pair1AmountFiat = 5.23; 
    data.pair2Amount = 25.957536;
    data.pair2AmountFiat = 1.0; // USD = EUR
    data.feesCoin = "LINK";
    data.comision = 1.0;
    data.comisionFiat = 5.23;
    data.comments = "Venta Full Use case";
    data.type = "Venta";
    data.status = "";
    data.date = "31/12/2021 23:59:59";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values

    // Check retired amount
    wops = brokerManager->getLastNWalletOperation(2);
    QCOMPARE(true, wops.at(1)->getCoin() == "LINK");
    QCOMPARE(true, wops.at(1)->getAmount() == 11.004567);
    QCOMPARE(true, wops.at(1)->getAvailable() == 5.041367);
    QCOMPARE(true, wops.at(1)->getRetired() == 5.9632);
    QCOMPARE(true, wops.at(1)->getFiatPrice() == 6.24);
    QCOMPARE(true, wops.at(1)->getDateTimeUtc().toString() == "jue. dic. 30 22:59:00 2021");

    QCOMPARE(true, wops.at(0)->getCoin() == "USD");
    QCOMPARE(true, wops.at(0)->getRetired() == 0);
    QCOMPARE(true, wops.at(0)->getAvailable() == 25.957536);
    QCOMPARE(true, wops.at(0)->getDateTimeUtc().toString() == "vie. dic. 31 22:59:00 2021");

    auto op = brokerManager->getLastOperation();
    QCOMPARE(true, op->getPair1() == "LINK");
    QCOMPARE(true, op->getPair2() == "USD");
    QCOMPARE(true, op->getPair1Amount() ==  4.9632);
    QCOMPARE(true, op->getPair2Amount() == 25.957536);
    auto ganancia = (4.9632 + 1.0) * (5.23 - 6.24);
    QCOMPARE(true, op->getGanancia() < 0);
    QCOMPARE(true, op->getGanancia() == ganancia);
    QCOMPARE(true, op->getDate() == "vie. dic. 31 23:59:59 2021");

    data.walletID1 = 24;
    data.walletID2 = 20;
    data.pair1Amount = 3.5267;
    data.pair1AmountFiat = 7.23; 
    data.pair2Amount = 25.498041;
    data.pair2AmountFiat = 1.0; // USD = EUR
    data.feesCoin = "LINK";
    data.comision = 1.0;
    data.comisionFiat = 7.23;
    data.comments = "Venta 2 Full Use case";
    data.type = "Venta";
    data.status = "";
    data.date = "01/01/2022 02:25:00";
    QCOMPARE(1, brokerManager->newOperation(data, wOpsModified)); // compare two values

    // Check retired amount
    wops = brokerManager->getLastNWalletOperation(3);
    QCOMPARE(true, wops.at(2)->getCoin() == "LINK");
    QCOMPARE(true, wops.at(2)->getAmount() == 11.004567);
    QCOMPARE(true, QString::number(wops.at(2)->getAvailable()) == "0.514667");
    QCOMPARE(true, QString::number(wops.at(2)->getRetired()) == "10.4899");
    QCOMPARE(true, wops.at(2)->getFiatPrice() == 6.24);
    QCOMPARE(true, wops.at(2)->getDateTimeUtc().toString() == "jue. dic. 30 22:59:00 2021");

    QCOMPARE(true, wops.at(0)->getCoin() == "USD");
    QCOMPARE(true, wops.at(0)->getRetired() == 0);
    QCOMPARE(true, wops.at(0)->getAvailable() == 25.498041);
    QCOMPARE(true, wops.at(0)->getDateTimeUtc().toString() == "sáb. ene. 1 01:25:00 2022");

    op = brokerManager->getLastOperation();
    QCOMPARE(true, op->getPair1() == "LINK");
    QCOMPARE(true, op->getPair2() == "USD");
    QCOMPARE(true, op->getPair1Amount() ==  3.5267);
    QCOMPARE(true, op->getPair2Amount() == 25.498041);
    ganancia = (3.5267 + 1.0) * (7.23 - 6.24);
    QCOMPARE(true, op->getGanancia() > 0);
    QCOMPARE(true, op->getGanancia() == ganancia);
    QCOMPARE(true, op->getDate() == "sáb. ene. 1 02:25:00 2022");
}

void BrokerManagerTest::newDateCoversion()
{
    //TODO
}

void BrokerManagerTest::newOperationWallet1NotExist()
{
    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(true, brokerManager->newDeposit(13, 100.0, 0.0, "deposit 1", "")); // compare two values

    WalletOperation::OperationData data;
    data.walletID1 = 99;
    data.walletID2 = 1;
    data.pair1Amount = 90.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 0.03;
    data.pair2AmountFiat = 3000.0;
    data.feesCoin = "EUR";
    data.comision = 10.0;
    data.comisionFiat = 1.0;
    data.comments = "Compra 1";
    data.type = "Compra";
    data.status = "";
    data.date = "02/10/2022 10:00:00";
    QCOMPARE(3, brokerManager->newOperation(data, wOpsModified)); // compare two values
}

void BrokerManagerTest::newOperationWallet2NotExist()
{
    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(true, brokerManager->newDeposit(13, 100.0, 0.0, "deposit 1", "")); // compare two values


    WalletOperation::OperationData data;
    data.walletID1 = 99;
    data.walletID2 = 99;
    data.pair1Amount = 90.0;
    data.pair1AmountFiat = 1.0;
    data.pair2Amount = 0.03;
    data.pair2AmountFiat = 3000.0;
    data.feesCoin = "EUR";
    data.comision = 10.0;
    data.comisionFiat = 1.0;
    data.comments = "Compra 1";
    data.type = "Compra";
    data.status = "";
    data.date = "02/10/2022 10:00:00";
    QCOMPARE(3, brokerManager->newOperation(data, wOpsModified)); // compare two values
}

void BrokerManagerTest::updateTimeUTC()
{
    QSqlQuery query(DBLocal::getDb());

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (7, 10,0,10, \"lun. nov. 28 21:52:28 2022\")");
    query.exec();

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (7, 10,0,10, \"mar. jun. 28 21:52:28 2022\")");
    query.exec();

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (10, 10,0,10, \"lun. nov. 28 21:52:28 2022\")");
    query.exec();

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (10, 10,0,10, \"mar. jun. 28 21:52:28 2022\")");
    query.exec();

    DBLocal::GetInstance()->getWalletOperations("7");
    DBLocal::GetInstance()->getWalletOperations("10");

    auto wops = DBLocal::GetInstance()->getWalletOperations("7");
    auto w1 = wops->at(wops->size()-1);
    auto w2 = wops->at(wops->size()-2);

    query.prepare("select datetimeUTC from WalletOperations ORDER BY id DESC limit 4");
    query.exec();

    int index = 0;
    while(query.next())
    {
        if(index == 0)
            QCOMPARE(true, query.value(0).toString() == "2022-06-28T19:52:28.000");
        if(index == 1)
            QCOMPARE(true, query.value(0).toString() == "2022-11-28T20:52:28.000");
        if(index == 2)
            QCOMPARE(true, query.value(0).toString() == "2022-06-28T21:52:28.000");
        if(index == 3)
            QCOMPARE(true, query.value(0).toString() == "2022-11-28T21:52:28.000");
        index++;
    }
}

void BrokerManagerTest::cleanupTestCase()
{
    qDebug("Clean BrokerManagerTest");
}

QTEST_MAIN(BrokerManagerTest)
#include "BrokerManagerTest.moc"