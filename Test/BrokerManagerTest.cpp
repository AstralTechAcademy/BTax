#include <QTest>
#include <tuple>
#include "BrokerManager.h"
#include "Broker.h"
#include "SQLManager.h"
#include "Utils.h"

class BrokerManagerTest: public QObject
{
Q_OBJECT

private slots:
    void initTestCase();
    void loadTestData();
    void newAsset();
    void cleanupTestCase();
    void loadExchangesFromDB();
    void newAssetIfNotExist();
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
    ExchangesModel exchangesModel;
    NotificationManager notificationManager;
    BrokerManager* brokerManager;
};

void BrokerManagerTest::initTestCase()
{
    QString version = "1.1.0";
    qDebug("Called before everything else.");
    QQmlApplicationEngine engine;
    Broker* broker = Broker::getInstance(SQLManager::GetInstance()->getServer(), version, SQLManager::GetInstance()->getDatabase());
    engine.rootContext()->setContextProperty("BrokerImpl", broker);

    QCOMPARE(true, broker->openDatabase());
}

void BrokerManagerTest::loadTestData()
{
    QSqlQuery query(SQLManager::getDb());

    query.prepare("drop table  WalletOperations, Operations, Withdraws, Deposits, Wallets, Coins, Version, Exchanges, AssetType, Users");
    query.exec();

    SQLManager::GetInstance()->init();
    QCOMPARE(true, SQLManager::GetInstance()->registerUser("user1"));

    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("4", "Binance", 1)); // ID = 1 //ETH
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("4", "B2M", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("4", "Coinbase", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("4", "Crypto", 1));

    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("5", "Binance", 1)); // ID = 5 //ADA
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("5", "B2M", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("5", "Coinbase", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("5", "Crypto", 1));

    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("3", "Binance", 1)); // ID = 9 //BTC
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("3", "B2M", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("3", "Coinbase", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("3", "Crypto", 1));

    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("1", "Binance", 1)); // ID = 13 //EUR
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("1", "B2M", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("1", "Coinbase", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("1", "Crypto", 1));
    
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("2", "Binance", 1)); // ID = 17 //USD
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("2", "B2M", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("2", "Coinbase", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("2", "Bitpanda", 1)); // ID = 20

    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("15", "Binance", 1)); // ID = 21 //LINK
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("15", "B2M", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("15", "Coinbase", 1));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("15", "Bitpanda", 1)); // ID = 24

    brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel, &exchangesModel);
    brokerManager->load();

}

void BrokerManagerTest::newAsset()
{
    QCOMPARE(true, brokerManager->newAsset("stock", "prueba", "#FFFFFF")); // compare two values
}

void BrokerManagerTest::newAssetIfNotExist()
{
    QCOMPARE(true, brokerManager->newAssetIfNotExist("crypto", "MONEDA1", "#FFFFFFF")); // No exist -> Create
    QCOMPARE(false, brokerManager->newAssetIfNotExist("crypto", "MONEDA1", "#FFFFFFF")); // Already created
}

void BrokerManagerTest::loadExchangesFromDB()
{
    brokerManager->loadExchangesFromDB();
    QCOMPARE(true, exchangesModel.exchanges().count() == 9);
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


/*
 * Description: Two ADA wallets (diferent exchange) have bought diferent amount of ADA in diferent hour.
 *  FIFO algorithm is applied and earning is calculated.
*/
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
        
    /*******************************************************************************/
    /*******************************************************************************/
    /******************************** First Sale  ADA-B2M **************************/
    /*******************************************************************************/
    /*******************************************************************************/

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

    // Check that FIFO algortihm has been used. 
    // ADA-Binance has the oldest coins, so this coins are sold before.
    QCOMPARE(true, wOpsModified.size() == 1);
    QCOMPARE(true, QString::number(wOpsModified[0].getRetired()) == "68");
    QCOMPARE(true, QString::number(wOpsModified[0].getAvailable()) == "116.805");

    /*******************************************************************************/
    /*******************************************************************************/
    /******************************** Second Sale  ADA-B2M *************************/
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

    // Check that FIFO algortihm has been used. 
    // ADA-Binance has the oldest coins, so this coins are sold before.
    QCOMPARE(true, wOpsModified.size() == 1);
    QCOMPARE(true, QString::number(wOpsModified[0].getRetired()) == "88");
    QCOMPARE(true, QString::number(wOpsModified[0].getAvailable()) == "96.8049");

    // Check that FIFO algortihm has been used. 
    // ADA-B2M has all amount available.
    wallet = SQLManager::GetInstance()->getWalletOperations("6");
    ws = wallet.value();
    QCOMPARE(true, ws.size() == 1);
    QCOMPARE(true, QString::number(ws[0]->getRetired()) == "0");
    QCOMPARE(true, QString::number(ws[0]->getAvailable()) == "147.493");

    // Check that EUR-B2M has amount available bacause of sales.
    wallet = SQLManager::GetInstance()->getWalletOperations("14");
    ws = wallet.value();
    QCOMPARE(true, ws.size() == 3);
    QCOMPARE(true, QString::number(ws[1]->getRetired()) == "0");
    QCOMPARE(true, QString::number(ws[1]->getAvailable()) == "20.2362");
    QCOMPARE(true, QString::number(ws[2]->getRetired()) == "0");
    QCOMPARE(true, QString::number(ws[2]->getAvailable()) == "6.978");

}

/*
 * Description: Calculate average cost of each individual wallet. 
    In one wallet two buy operation is performed, average cost is calculated and then one sell operation
    is perfomed, average cost is calculated.

    Othe wallet one buy operation is performed, average cost is calculated.
*/
void BrokerManagerTest::getAverage()
{
    std::vector<WalletOperation> wOpsModified;
    WalletOperation::OperationData data;
    auto average="";
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

    auto ws = SQLManager::GetInstance()->getCryptoWallets(BrokerManager::userID);

    for(auto w : ws.value())
    {

        if(w->getWalletID() == 9)
        {
            average = "49461.9"; // = (56.0+80.0)/(0.0008582604 + 0.0018913155);
            QCOMPARE(true, QString::number(w->getAverageCost()) == average); // Este test no funciona porque los decimales no son precisos
        }

        if(w->getWalletID() == 10)
        {
            average = "33248.3";
            QCOMPARE(true, QString::number(w->getAverageCost()) == average);
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

    ws = SQLManager::GetInstance()->getCryptoWallets(BrokerManager::userID);
    for(auto w : ws.value())
    {
        if(w->getWalletID() == 9)
        {
            average = "48595.9"; // = (46.6694+80.0)/(0.00071526043+0.00189131555);
            qDebug() <<  QString::number(w->getAverageCost())  << " " <<average;
            QCOMPARE(true, QString::number(w->getAverageCost()) == average);
        }
    }
}


void BrokerManagerTest::getGlobalAverage()
{

}

void BrokerManagerTest::calculatePortfolio()
{

}

/*
 * Description: Transfer from one wallet to another. 
    WalletOperations register has to be modified keeping the same values in the new walletoperation register. 
    A transfer does not change the operation datetime
*/
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
        QCOMPARE(true, BTime::toString(operations[index]->getDate()) == BTime::toString(wOpsModified[indexModified].getDate()));
        QCOMPARE(true, BTime::toString(operations[index]->getDate(), QLocale::Spanish, EN_DateFormat::DMYhms) != "03/10/2022 10:00:00");
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

    auto w = std::get<1>(SQLManager::GetInstance()->getWallet(24));
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

    // Check retired amount. FIFO algorithm.
    wops = brokerManager->getLastNWalletOperation(2);
    QCOMPARE(true, wops.at(1)->getCoin() == "LINK");
    QCOMPARE(true, wops.at(1)->getAmount() == 11.004567);
    QCOMPARE(true, wops.at(1)->getAvailable() == 5.041367);
    QCOMPARE(true, wops.at(1)->getRetired() == 5.9632);
    QCOMPARE(true, wops.at(1)->getFiatPrice() == 6.24);
    QCOMPARE(true, BTime::toString(wops.at(1)->getDateTimeUtc()) == "jue. dic. 30 22:59:00 2021");

    QCOMPARE(true, wops.at(0)->getCoin() == "USD");
    QCOMPARE(true, wops.at(0)->getRetired() == 0);
    QCOMPARE(true, wops.at(0)->getAvailable() == 25.957536);
    QCOMPARE(true, BTime::toString(wops.at(0)->getDateTimeUtc()) == "vie. dic. 31 22:59:00 2021");

    auto op = brokerManager->getLastOperation();
    QCOMPARE(true, op->getPair1() == "LINK");
    QCOMPARE(true, op->getPair2() == "USD");
    QCOMPARE(true, op->getPair1Amount() ==  4.9632);
    QCOMPARE(true, op->getPair2Amount() == 25.957536);
    auto ganancia = (4.9632 + 1.0) * (5.23 - 6.24);
    QCOMPARE(true, op->getGanancia() < 0);
    QCOMPARE(true, op->getGanancia() == ganancia);
    QCOMPARE(true, op->getDate() == "vie. dic. 31 23:59:59 2021");
    qDebug() << op->getDateTime() <<" "<< BTime::toString(op->getDateTime(), QLocale::Spanish, EN_DateFormat::DMYhms);
    QCOMPARE(true, BTime::toString(op->getDateTime(), QLocale::Spanish, EN_DateFormat::DMYhms) == "31/12/2021 23:59:59");


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
    QCOMPARE(true, BTime::toString(wops.at(2)->getDateTimeUtc()) == "jue. dic. 30 22:59:00 2021");

    QCOMPARE(true, wops.at(0)->getCoin() == "USD");
    QCOMPARE(true, wops.at(0)->getRetired() == 0);
    QCOMPARE(true, wops.at(0)->getAvailable() == 25.498041);
    QCOMPARE(true, BTime::toString(wops.at(0)->getDateTimeUtc()) == "sáb. ene. 1 01:25:00 2022");

    op = brokerManager->getLastOperation();
    QCOMPARE(true, op->getPair1() == "LINK");
    QCOMPARE(true, op->getPair2() == "USD");
    QCOMPARE(true, op->getPair1Amount() ==  3.5267);
    QCOMPARE(true, op->getPair2Amount() == 25.498041);
    ganancia = (3.5267 + 1.0) * (7.23 - 6.24);
    QCOMPARE(true, op->getGanancia() > 0);
    QCOMPARE(true, op->getGanancia() == ganancia);
    QCOMPARE(true, op->getDate() == "sáb. ene. 1 02:25:00 2022");
    qDebug() << op->getDateTime() <<" "<< BTime::toString(op->getDateTime(), QLocale::Spanish, EN_DateFormat::DMYhms);

    QCOMPARE(true, BTime::toString(op->getDateTime(), QLocale::Spanish, EN_DateFormat::DMYhms) == "01/01/2022 02:25:00");

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

/*
 * Description: Calculate UTC+0 time in WalletOperations table if the value does not exist.
    The calculation is based on the exchange, Bit2Me exchange date time is Spanish timezone and Binance 
    or other exchanges no conversion is required.
*/
void BrokerManagerTest::updateTimeUTC()
{
    QSqlQuery query(SQLManager::getDb());

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (7, 10,0,10, \"lun. nov. 28 21:52:28 2022\")");
    query.exec();

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (7, 10,0,10, \"mar. jun. 28 21:52:28 2022\")");
    query.exec();

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (10, 10,0,10, \"lun. nov. 28 21:52:28 2022\")");
    query.exec();

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (10, 10,0,10, \"mar. jun. 28 21:52:28 2022\")");
    query.exec();

    SQLManager::GetInstance()->getWalletOperations("7");
    SQLManager::GetInstance()->getWalletOperations("10");

    auto wops = SQLManager::GetInstance()->getWalletOperations("7");
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

QTEST_GUILESS_MAIN(BrokerManagerTest)
#include "BrokerManagerTest.moc"