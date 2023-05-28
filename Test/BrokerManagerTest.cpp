#include <QTest>
#include <tuple>
#include "BrokerManager.h"
#include "Broker.h"
#include "Utils.h"
#include "Test_Utils.h"

class BrokerManagerTest: public QObject
{
Q_OBJECT

private slots:
    void initTestCase();
    void loadTestData();
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
    Broker* broker;
    UsersModel usersModel;
    OperationsModel operationsModel;
    WalletsModel walletsModel;
    WalletsModel walletsModelAll;
    WalletsPercModel walletsPercModel;
    CoinsModel coinsModel;
    AssetTypeModel assetTypeModel;
    ExchangesModel exchangesModel;
    NotificationManager* notificationManager = NotificationManager::getInstance();
    BrokerManager* brokerManager;
    uint16_t userID = 0;

///// COMMON TEST DATA
    uint16_t eurID = 0;
    uint16_t ethID = 0;    
    uint16_t adaID = 0;
    uint16_t usdID = 0;
    uint16_t linkID = 0;
    uint16_t btcID = 0;
    
    uint16_t CoinbaseEur = 0;
    uint16_t B2MEur = 0;
    uint16_t CoinbaseAda = 0;
    uint16_t B2MAda = 0;
    uint16_t B2MEth = 0;
    uint16_t B2MBtc = 0;
    uint16_t BinanceEur = 0;
    uint16_t BinanceEth = 0;
    uint16_t BitpandaUsd = 0;
    uint16_t BitpandaLink = 0;
    uint16_t BinanceBtc = 0;
////////////    
};

void BrokerManagerTest::initTestCase()
{
    QString version = "1.1.0";
    qDebug("Called before everything else.");
    QQmlApplicationEngine engine;
    broker = Broker::getInstance(version);
    engine.rootContext()->setContextProperty("BrokerImpl", broker);

    broker->openDatabase();
}

void BrokerManagerTest::loadTestData()
{
    QSqlQuery query(SQLManager::getDb());

    deleteTablesData(query);

    QCOMPARE(true, SQLManager::GetInstance()->registerUser("user1"));

    userID = SQLManager::GetInstance()->getUserID("user1");

    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("4", "Binance", userID)); // ID = 1 //ETH
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("4", "B2M", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("4", "Coinbase", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("4", "Crypto", userID));

    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("5", "Binance", userID)); // ID = 5 //ADA
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("5", "B2M", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("5", "Coinbase", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("5", "Crypto", userID));

    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("3", "Binance", userID)); // ID = 9 //BTC
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("3", "B2M", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("3", "Coinbase", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("3", "Crypto", userID));

    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("1", "Binance", userID)); // ID = 13 //EUR
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("1", "B2M", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("1", "Coinbase", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("1", "Crypto", userID));
    
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("2", "Binance", userID)); // ID = 17 //USD
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("2", "B2M", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("2", "Coinbase", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("2", "Bitpanda", userID)); // ID = 20

    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("15", "Binance", userID)); // ID = 21 //LINK
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("15", "B2M", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("15", "Coinbase", userID));
    QCOMPARE(true, (bool) SQLManager::GetInstance()->addWallet("15", "Bitpanda", userID)); // ID = 24

    brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel, &exchangesModel);
    brokerManager->load();

///// COMMON TEST DATA
    auto eur = SQLManager::GetInstance()->getCoin("EUR");
    auto eth = SQLManager::GetInstance()->getCoin("ETH");
    auto ada = SQLManager::GetInstance()->getCoin("ADA");
    auto usd = SQLManager::GetInstance()->getCoin("USD");
    auto link = SQLManager::GetInstance()->getCoin("LINK");
    auto btc = SQLManager::GetInstance()->getCoin("BTC");
    QCOMPARE(true, eur != std::nullopt);
    QCOMPARE(true, ada != std::nullopt);
    QCOMPARE(true, eth != std::nullopt);
    QCOMPARE(true, usd != std::nullopt);
    QCOMPARE(true, link != std::nullopt);
    QCOMPARE(true, btc != std::nullopt);
    eurID = std::get<0>(eur.value());
    ethID = std::get<0>(eth.value());
    adaID = std::get<0>(ada.value());
    usdID = std::get<0>(usd.value());
    linkID = std::get<0>(link.value());
    btcID = std::get<0>(link.value());
    
    CoinbaseEur = SQLManager::GetInstance()->getWalletID(userID, "Coinbase", QString::number(eurID));
    B2MEur = SQLManager::GetInstance()->getWalletID(userID, "B2M", QString::number(eurID));
    CoinbaseAda = SQLManager::GetInstance()->getWalletID(userID, "Coinbase", QString::number(adaID));
    B2MAda = SQLManager::GetInstance()->getWalletID(userID, "B2M", QString::number(adaID));
    B2MEth = SQLManager::GetInstance()->getWalletID(userID, "B2M", QString::number(ethID));
    B2MBtc = SQLManager::GetInstance()->getWalletID(userID, "B2M", QString::number(btcID));

    BinanceEur = SQLManager::GetInstance()->getWalletID(userID, "Binance", QString::number(eurID));
    BinanceEth = SQLManager::GetInstance()->getWalletID(userID, "Binance", QString::number(ethID));
    BinanceBtc = SQLManager::GetInstance()->getWalletID(userID, "Binance", QString::number(btcID));
    BitpandaUsd = SQLManager::GetInstance()->getWalletID(userID, "Bitpanda", QString::number(usdID));
    BitpandaLink = SQLManager::GetInstance()->getWalletID(userID, "Bitpanda", QString::number(linkID));
    QCOMPARE(true, BinanceEur > 0);
    QCOMPARE(true, BinanceEth > 0);
    QCOMPARE(true, CoinbaseEur > 0);
    QCOMPARE(true, B2MEur > 0);
    QCOMPARE(true, B2MEth > 0);
    QCOMPARE(true, B2MBtc > 0);
    QCOMPARE(true, CoinbaseAda > 0);
    QCOMPARE(true, B2MAda > 0);    
    QCOMPARE(true, BitpandaUsd > 0);    
    QCOMPARE(true, BitpandaLink > 0);
    QCOMPARE(true, BinanceBtc > 0); 

////////////    
    

}

void BrokerManagerTest::newAssetIfNotExist()
{
    QSqlQuery query(SQLManager::getDb());
    query.prepare("DELETE FROM Coins WHERE name = \"MONEDA1\"");
    query.exec();
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

    QCOMPARE(true, brokerManager->newDeposit(BinanceEur, 150.0, 0.0, "deposit 1", "")); // compare two values
    
    data.walletID1 = BinanceEur;
    data.walletID2 = BinanceEth;
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
        
    data.walletID1 = BinanceEur;
    data.walletID2 = BinanceEth;
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

    QCOMPARE(true, brokerManager->newDeposit(CoinbaseEur, 133.0, 0.0, "deposit 2", "")); // compare two values
    QCOMPARE(true, brokerManager->newDeposit(B2MEur, 133.0, 0.0, "deposit 2", "")); // compare two values

    data.walletID1 = CoinbaseEur;
    data.walletID2 = CoinbaseAda;
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

    data.walletID1 = B2MEur;
    data.walletID2 = B2MAda;
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

    data.walletID1 = B2MAda;
    data.walletID2 = B2MEur;
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

    auto ganancia = ((58.0) * (0.3489 - 0.487)) - (10.0 * 0.3489);
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
    data.walletID1 = B2MAda;
    data.walletID2 = B2MEur;
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
    wallet = SQLManager::GetInstance()->getWalletOperations(QString::number(B2MAda));
    ws = wallet.value();
    QCOMPARE(true, ws.size() == 1);
    QCOMPARE(true, QString::number(ws[0]->getRetired()) == "0");
    QCOMPARE(true, QString::number(ws[0]->getAvailable()) == "147.493");

    // Check that EUR-B2M has amount available bacause of sales.
    wallet = SQLManager::GetInstance()->getWalletOperations(QString::number(B2MEur));
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
    QCOMPARE(true, brokerManager->newDeposit(BinanceEur, 177.0, 0.0, "deposit getAverage", "")); // compare two values
    QCOMPARE(true, brokerManager->newDeposit(B2MEur, 177.0, 0.0, "deposit getAverage", "")); // compare two values
    data.walletID1 = BinanceEur;
    data.walletID2 = BinanceBtc;
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
    
    data.walletID1 = BinanceEur;
    data.walletID2 = BinanceBtc;
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
    
    data.walletID1 = B2MEur;
    data.walletID2 = B2MBtc;
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

        if(w->getWalletID() == BinanceBtc)
        {
            average = "49461.9"; // = (56.0+80.0)/(0.0008582604 + 0.0018913155);
            QCOMPARE(true, QString::number(w->getAverageCost()) == average); // Este test no funciona porque los decimales no son precisos
        }

        if(w->getWalletID() == B2MBtc)
        {
            average = "33248.3";
            QCOMPARE(true, QString::number(w->getAverageCost()) == average);
        }
    }

    data.walletID1 = BinanceBtc;
    data.walletID2 = BinanceEth;
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
        if(w->getWalletID() == BinanceBtc)
        {
            average = "48595.9"; // = (46.6694+80.0)/(0.00071526043+0.00189131555);
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

    data.walletID1 = BinanceEth;
    data.walletID2 = B2MEth;
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
    QCOMPARE(true, brokerManager->newDeposit(BinanceEur, 150.0, 0.0, "deposit 1", "")); // compare two values
    WalletOperation::OperationData data;
    data.walletID1 = BinanceEur;
    data.walletID2 = BinanceEth;
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
        
    data.walletID1 = BinanceEur;
    data.walletID2 = BinanceEth;
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
    QCOMPARE(true, brokerManager->newDeposit(BitpandaUsd, 133.33333, 0.0, "deposit full use case", "")); // compare two values

    WalletOperation::OperationData data;
    data.walletID1 = BitpandaUsd;
    data.walletID2 = BitpandaLink;
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

    auto w = std::get<1>(SQLManager::GetInstance()->getWallet(BitpandaLink));
    QCOMPARE(true, QString::number(w->getAverageCost()) == "6.24");

    data.walletID1 = BitpandaLink;
    data.walletID2 = BitpandaUsd;
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
    auto ganancia = ((4.9632 ) * (5.23 - 6.24)) - (1.0 * 5.23);
    QCOMPARE(true, op->getGanancia() < 0);
    QCOMPARE(true, QString::number(op->getGanancia()) == QString::number(ganancia));
    QCOMPARE(true, op->getDate() == "vie. dic. 31 23:59:59 2021");
    QCOMPARE(true, BTime::toString(op->getDateTime(), QLocale::Spanish, EN_DateFormat::DMYhms) == "31/12/2021 23:59:59");


    data.walletID1 = BitpandaLink;
    data.walletID2 = BitpandaUsd;
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
    ganancia = ((3.5267) * (7.23 - 6.24)) - (1.0 * 7.23);
    QCOMPARE(true, op->getGanancia() < 0);
    QCOMPARE(true, op->getGanancia() == ganancia);
    QCOMPARE(true, op->getDate() == "sáb. ene. 1 02:25:00 2022");
    QCOMPARE(true, BTime::toString(op->getDateTime(), QLocale::Spanish, EN_DateFormat::DMYhms) == "01/01/2022 02:25:00");

}

void BrokerManagerTest::newDateCoversion()
{
    //TODO
}

void BrokerManagerTest::newOperationWallet1NotExist()
{
    std::vector<WalletOperation> wOpsModified;
    QCOMPARE(true, brokerManager->newDeposit(BinanceEur, 100.0, 0.0, "deposit 1", "")); // compare two values

    WalletOperation::OperationData data;
    data.walletID1 = 99;
    data.walletID2 = BinanceEth;
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
    QCOMPARE(true, brokerManager->newDeposit(BinanceEur, 100.0, 0.0, "deposit 1", "")); // compare two values


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

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (" + QString::number(CoinbaseAda) + ", " + QString::number(B2MBtc) + ",0,10, \"lun. nov. 28 21:52:28 2022\")");
    query.exec();

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (" + QString::number(CoinbaseAda) + ", " + QString::number(B2MBtc) + ",0,11, \"mar. jun. 28 21:52:28 2022\")");
    query.exec();

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (" + QString::number(B2MBtc) + ", "  + QString::number(B2MBtc) + ",0,12, \"lun. nov. 28 21:52:28 2022\")");
    query.exec();

    query.prepare("insert into WalletOperations(wallet,amount,retired,available,date) VALUES (" + QString::number(B2MBtc) + ", "  + QString::number(B2MBtc) + ",0,13, \"mar. jun. 28 21:52:28 2022\")");
    query.exec();

    SQLManager::GetInstance()->getWalletOperations(QString::number(CoinbaseAda));
    SQLManager::GetInstance()->getWalletOperations(QString::number(B2MBtc));

    query.prepare("select datetimeUTC from WalletOperations ORDER BY id DESC limit 4");
    query.exec();

    int index = 0;
    while(query.next())
    {
        if(index == 0)
            QCOMPARE(true, query.value(0).toString() == "2022-06-28T19:52:28.000");  //B2M
        if(index == 1)
            QCOMPARE(true, query.value(0).toString() == "2022-11-28T20:52:28.000"); //B2M
        if(index == 2)
            QCOMPARE(true, query.value(0).toString() == "2022-06-28T21:52:28.000"); //Coinbase
        if(index == 3)
            QCOMPARE(true, query.value(0).toString() == "2022-11-28T21:52:28.000"); //Coinbase
        index++;
    }
}

void BrokerManagerTest::cleanupTestCase()
{
    qDebug("Clean BrokerManagerTest");
}

QTEST_GUILESS_MAIN(BrokerManagerTest)
#include "BrokerManagerTest.moc"