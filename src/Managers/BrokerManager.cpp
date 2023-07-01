//
// Created by gabridc on 26/11/21.
//

#include "BrokerManager.h"
#include "Broker.h"
#include "Utils.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "UsersModel.h"
#include "Validators/VOperation.h"


BrokerManager::BrokerManager(const QObject* parent, OperationsModel*const operationsModel, WalletsModel*const walletsModel, 
                            WalletsModel*const walletsModelAll, WalletsPercModel*const walletsPercModel, CoinsModel*const coinsModel, 
                            AssetTypeModel*const assetTypesModel,  ExchangesModel*const exchangesModel)
{
    parent = 0;
    operationsModel_ = operationsModel;
    walletsModel_ = walletsModel;
    walletsModelAll_ = walletsModelAll;
    walletsModelPerc_ = walletsPercModel;
    coinsModel_ = coinsModel;
    exchangesModel_ = exchangesModel;
    assetTypesModel_ = assetTypesModel;


    connect(WalletsFilterBarManager::getInstance(), SIGNAL(assetCategorySelected(const QString&)), this ,SLOT(updateAssetTypeModel(const QString&)));
}


bool BrokerManager::newUser(const QString& name)
{
    return SQLManager::GetInstance()->registerUser(name);
}

bool BrokerManager::newDeposit(const int walletID, double amount, double fees,
                const QString comment, QString date)
{
    auto locale = QLocale(QLocale::Spanish);
    QString format = "ddd. MMM. d hh:mm:ss yyyy";
    QDateTime dateTime;
    dateTime.setDate(QDate(1900, 1, 1));
    if(date == "")
        date =  locale.toString(QDateTime::currentDateTime(), format);
    else
    {

        dateTime.setDate(QDate(date.split(" ")[0].split("/")[2].toInt(),
                               date.split(" ")[0].split("/")[1].toInt(),
                               date.split(" ")[0].split("/")[0].toInt()));
        dateTime.setTime(QTime(date.split(" ")[1].split(":")[0].toInt(),
                               date.split(" ")[1].split(":")[1].toInt(),
                               date.split(" ")[1].split(":")[2].toInt()));

        date = locale.toString(dateTime, format);

    }

    auto [r1, wallet1] = SQLManager::GetInstance()->getWallet(walletID);
    if(!r1)
        return false;

    auto res = SQLManager::GetInstance()->depositOperation(walletID, wallet1->getExchange() ,amount, 1, fees, comment, date);

    loadWalletsFromDB(userID);
    if(res)
        emit depositCompleted();
    return res;
}

int BrokerManager::newOperation(const int walletID1,const int walletID2, double pair1Amount, double pair1AmountFiat,
                 double pair2Amount, double pair2AmountFiat, QString feesCoin, double comision, double comisionFiat, QString comments, QString type,
                 QString status, QString date)
{
    std::vector<WalletOperation> wOpsModified;
    WalletOperation::OperationData data;
    data.walletID1 = walletID1;
    data.walletID2 = walletID2;
    data.pair1Amount = pair1Amount;
    data.pair1AmountFiat = pair1AmountFiat;
    data.pair2Amount = pair2Amount;
    data.pair2AmountFiat = pair2AmountFiat;
    data.feesCoin = feesCoin;
    data.comision = comision;
    data.comisionFiat = comisionFiat;
    data.comments = comments;
    data.type = type;
    data.status = status;
    data.date = date;
    return newOperation(data , wOpsModified);

}


int BrokerManager::newOperation(WalletOperation::OperationData data, std::vector<WalletOperation>& wOpsModified)
{
    LOG_DEBUG("newOperation called");
    QDateTime dateTime;
    dateTime.setDate(QDate(1900, 1, 1));
    cnvStrToDateTime(data.date, dateTime);
    if(data.status == "")
        data.status = "Confirmed";

    double totalAmount = 0.0;

    auto [r1, wallet1] = SQLManager::GetInstance()->getWallet(data.walletID1);
    auto [r2, wallet2] = SQLManager::GetInstance()->getWallet(data.walletID2);

    if(r1 and r2)
    {
        auto coin = findCoin(wallet1->getCoin());
        if(coin == std::nullopt)
        {
            LOG_ERROR("Coin %s doest not exist", qPrintable(wallet1->getCoin()));
            return static_cast<int>( NewOperationRes::COIN_NOT_FOUND);
        }

        if(data.feesCoin == wallet1->getCoin())
        {
            //std::cout << "Same coin fees"<< std::endl;
            totalAmount = data.pair1Amount + data.comision;  // Formula reviwed. OK.
        }
        else
        {
            //std::cout << "Not same coin fees. Calculation to same coin: " << (comision * comisionFiat) / pair1AmountFiat << std::endl;
            totalAmount = data.pair1Amount + (data.comision * data.comisionFiat) / data.pair1AmountFiat; // Formula reviwed. OK.
        }

        VOperation voperation(data, dateTime);
        if( voperation.validate() == false)
            return static_cast<int>( NewOperationRes::VALIDATION_ERROR);

         // Calculate Total Available amount
        std::optional<std::vector<WalletOperation*>> ws;
        if(data.type == "Transferencia" or coin.value()->type() == "fiat") 
            ws = getAvailableBalancesOrdered(QString::number(coin.value()->id()), wallet1->getExchange());   //Se comprueba que haya saldo suficiente para la compra antes de proceder
        else
            ws = getAvailableBalancesOrdered(QString::number(coin.value()->id()));

         if(coin.value()->type() == "fiat")
        {          
            if (ws == std::nullopt)
            {
                LOG_ERROR("The coin %s doest not have wallets already created", qPrintable(wallet1->getCoin()));
                return static_cast<int>( NewOperationRes::ORI_WALLET_NOT_EXIST);
            }

            auto wallets = ws.value();
            double totalAvailableAmt = getAvailableAmounts(wallets);

            if(totalAvailableAmt >= totalAmount)
            {
                auto res = SQLManager::GetInstance()->registerOperationNew(wallets, data, wOpsModified);
                return res ? static_cast<int>( NewOperationRes::ADDED) :  static_cast<int>( NewOperationRes::NOT_ADDED);
            }
            else
            {
                LOG_ERROR("Not enough balance in the origin wallet to finish the operation. WalletID: %d", wallet1->getWalletID());
                emit NotificationManager::getInstance()->
                    newOperationError(QString("Not enough balance in the origin wallet to finish the operation"));
                return static_cast<int>( NewOperationRes::INSUF_BALANCE_ORI_WALLET);
            }
        }
        else
        {
            if (ws == std::nullopt)
            {
                LOG_ERROR("The coin %s doest not have wallets already created", qPrintable(wallet1->getCoin()));
                return static_cast<int>( NewOperationRes::ORI_WALLET_NOT_EXIST);
            }
            auto wallets = ws.value();
            double totalAvailableAmt = getAvailableAmounts(wallets);

            if(totalAvailableAmt >= totalAmount)
            {
                auto res = SQLManager::GetInstance()->registerOperationNew(wallets, data, wOpsModified);
                return res ? static_cast<int>( NewOperationRes::ADDED) :  static_cast<int>( NewOperationRes::NOT_ADDED);
            }
            else
            {
                LOG_ERROR("Not enough balance in the origin wallet to finish the operation. WalletID: %d", wallet1->getWalletID());
                return static_cast<int>( NewOperationRes::INSUF_BALANCE_ORI_WALLET);
            }
        }
    }
    else
    {
        LOG_ERROR("Origin wallet does not exist. WalletID: %d", wallet1->getWalletID());
        if(!r1)
            return static_cast<int>( NewOperationRes::ORI_WALLET_NOT_EXIST);
        if(!r2)
            return static_cast<int>( NewOperationRes::DEST_WALLET_NOT_EXIST);
    }
}

int BrokerManager::newTransfer(const int walletID1,const int walletID2, double walletOAmount, double walletDAmount, QString feesCoin, 
                        double comision, double comisionFiat, QString comments, QString status, QString date)
{

    std::vector<WalletOperation> wOpsModified;
    WalletOperation::OperationData data;
    data.walletID1 = walletID1;
    data.walletID2 = walletID2;
    data.pair1Amount = walletOAmount;
    data.pair1AmountFiat = 0;
    data.pair2Amount = walletDAmount;
    data.pair2AmountFiat = 0;
    data.feesCoin = feesCoin;
    data.comision = comision;
    data.comisionFiat = comisionFiat;
    data.comments = comments;
    data.type = "Transferencia";
    data.status = status;
    data.date = date;
    return newOperation(data , wOpsModified);
}

int BrokerManager::newTransfer(WalletOperation::OperationData data, std::vector<WalletOperation>& wOpsModified)
{
    LOG_DEBUG("newTransfer called");
    bool result = false;
    QDateTime dateTime;
    dateTime.setDate(QDate(1900, 1, 1));
    cnvStrToDateTime(data.date, dateTime);


    return result;
}


int BrokerManager::newOperation(const QString& exchange, std::shared_ptr<Operation> operation, std::vector<WalletOperation>& wOpsModified)
{
    auto res = setWallets(exchange, operation);
    if(res != static_cast<int>(NewOperationRes::OK)) return res; 

    if(checkDuplicity(exchange, operation))
    {
        LOG_WARN("The operation already exist. Operation date: %s",  qPrintable(operation->getDate()));
        return static_cast<int>( NewOperationRes::ALREADY_ADDED);
    }

    WalletOperation::OperationData data;
    data.fromOperation(operation);
    return newOperation(data, wOpsModified);
}

bool BrokerManager::newAssetIfNotExist(const QString& type, const QString& name, const QString& color)
{
    if(findCoin(name) != std::nullopt)
        return false;
    
    auto res = SQLManager::GetInstance()->registerAsset(type, name, color);
    if(res)
        loadCoinsFromDB();
    return res;
}

bool BrokerManager::newAsset(const QString& type, const QString& name, const QString& color)
{
    auto assets = coinsModel_->coins();
    auto exist = std::find_if(assets.begin(), assets.end(),
                              [&](Coin* coin){return coin->name() == name;});
    if(exist != assets.end())
    {
        LOG_WARN("The asset %s already exist",  qPrintable(name));
        return false;
    }

    auto res = SQLManager::GetInstance()->registerAsset(type, name, color);

    if(res)
        loadCoinsFromDB();
    return res;
}

bool BrokerManager::addWalletIfNotExist(const QString exchange, 
                                        const QString coinName)
{
    bool exist = false;

    auto res = SQLManager::GetInstance()->getWallets();
    if(std::get<0>(res) == false)
    {
        return (SQLManager::GetInstance()->addWallet(coinName, exchange, userID) > 0);
    }

    auto uname = SQLManager::GetInstance()->getUsername(userID);
    auto wallets = std::get<1>(res);
    for(auto w : wallets)
    {
        if(w->getCoin() == coinName && w->getExchange() == exchange && w->getUser() == uname)
            exist = true;
    }

    if(!exist)
    {
        auto res = findCoin(coinName);
        if(res == std::nullopt)
            return false;
        auto coin = res.value();
        return (SQLManager::GetInstance()->addWallet(QString::number(coin->id()), exchange, userID) > 0);
    }

    return exist;
}

bool BrokerManager::addWallet(const QString coinName, const QString exchange)
{
    bool exist = false;
    auto wallets = walletsModel_->wallets();
    for(auto w : wallets)
    {
        if(w->getCoin() == coinName && w->getExchange() == exchange)
            exist = true;
    }

    if(exist)
        return false;
    else
    {
        auto res = findCoin(coinName);
        if(res == std::nullopt)
            return false;
        auto coin = res.value();
        auto result = SQLManager::GetInstance()->addWallet(QString::number(coin->id()), exchange, userID);
        if ( result > 0)
        {
            loadWalletsFromDB(userID);
            return true;
        }
        else
            return false;

    }

}

uint32_t BrokerManager::getUserID(const QString& username)
{
    return SQLManager::GetInstance()->getUserID(username);
}


QStringList BrokerManager::getWalletsCBox(const QString& username)
{
    QStringList result;
    std::vector<Wallet *> wallets;
    if(username != "")
        wallets = std::get<1>(SQLManager::GetInstance()->getWallets(getUserID(username)));
    else
        wallets = std::get<1>(SQLManager::GetInstance()->getWallets(userID));

    if(wallets.empty() == false)
    {
        for(auto w : wallets)
        {
            result.push_back(QString::number(w->getWalletID()) + " " + w->getUser() + " " + w->getCoin() + " " + w->getExchange());
        }
    }

    return result;
}

void BrokerManager::setUserID(const QString& username)
{
    if(!username.isEmpty())
    {
        userID = getUserID(username);
        LOG_DEBUG("User ID: %d Username: %s" , userID, qPrintable(username));
        walletsModel_->clear();
        operationsModel_->clear();
        loadWalletsFromDB(userID);
        loadOperationsFromDB(userID);
    }
}

void BrokerManager::setYear(const QString& year)
{
    year_ = year.toUInt();
    if(userID != 0U && year_ > 0)
    {
        operationsModel_->clear();
        loadOperationsFromDB(userID, year_);
    }

}

void BrokerManager::loadOperationsFromDB(const uint32_t userID)
{
    auto operations = SQLManager::GetInstance()->getOperations(userID);
    if(std::get<0>(operations) == true)
    {
        for(auto op : std::get<1>(operations))
            operationsModel_->add(op);
    }

    LOG_DEBUG("Operations loaded");
}

void BrokerManager::loadOperationsFromDB(const uint32_t userID, const uint32_t year)
{
    auto operations = SQLManager::GetInstance()->getOperations(userID);
    if(std::get<0>(operations) == true)
    {
        for(auto op : std::get<1>(operations))
        {
            if(op->getDate().contains("/") == false && QDateTime::fromString(op->getDate()).date().year() == year)
                operationsModel_->add(op);
        }

    }
}

void BrokerManager::loadCoinsFromDB(void)
{
    coinsModel_->clear();
    auto coins = SQLManager::GetInstance()->getCoins();
    for(auto c : coins)
        coinsModel_->add(new Coin(std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c)));
}

void BrokerManager::loadExchangesFromDB(void)
{
    exchangesModel_->clear();
    auto exchanges = SQLManager::GetInstance()->getExchanges();
    for(auto e : exchanges)
        exchangesModel_->add(new Exchange(std::get<0>(e), std::get<1>(e), std::get<2>(e)));
}

void BrokerManager::loadAssetTypesFromDB(void)
{
    auto assets = SQLManager::GetInstance()->getAssetTypes();
    for(auto c : assets)
        assetTypesModel_->add(new AssetType(std::get<0>(c), std::get<1>(c)));
}

void BrokerManager::filterAssetTypes(const QString& category)
{
    assetTypesModel_->clear();
    auto assets = SQLManager::GetInstance()->getAssetTypes(category);
    for(auto c : assets)
        assetTypesModel_->add(new AssetType(std::get<0>(c), std::get<1>(c)));
    assetTypesModel_->updateLayout();
}

void BrokerManager::loadWalletsFromDB(const uint32_t userID, const QList<WalletsModel::AssetType> assetTypes)
{
    walletsModel_->clear();
    walletsModelAll_->clear();
    
    auto result = SQLManager::GetInstance()->getWallets(userID, assetTypes);
    if(std::get<0>(result) == true)
    {
        auto wallets = std::get<1>(result);
        for(auto w : wallets)
        {
            if(w->getAmount() > 0.00000000000)
            {
                walletsModel_->add(w);
            }
            walletsModelAll_->add(w);
        }
    }

    walletsModel_->orderBy(WalletsModel::Attribute::PORTFOLIO, WalletsModel::Order::ASC);
    //walletsModel_->orderBy(WalletsModel::Attribute::TYPE, WalletsModel::Order::FIAT_FIRST);
    walletsModelAll_->orderBy(WalletsModel::Attribute::TYPE, WalletsModel::Order::FIAT_FIRST);

    groupCoinBySymbol();
    setCoinPtrInWallets();
}

void BrokerManager::loadDepositsFromDB(const uint32_t userID)
{
    double totalInvested = 0.0;
    auto result = SQLManager::GetInstance()->getDeposits(userID);

    if(result == std::nullopt)
        return;

    auto deposits = result.value();
    for(auto d : deposits)
        totalInvested += d->getAmount();
    std::cout << "Total Invested " << totalInvested << std::endl;
}

void BrokerManager::groupCoinBySymbol(void)
{
    walletsModelPerc_->clear();
    auto index = 0;
    for(index = 0; index <  walletsModel_->count() ; index++ )
    {
        auto coin = findCoin(walletsModel_->getCoin(index));

        if(coin != std::nullopt && coin.value()->type() != "fiat")
        {
            bool exist = false;
            auto index2 = 0;
            for(index2 = 0; index2 <  walletsModelPerc_->count() && exist == false ; index2++ )
            {
                exist = walletsModelPerc_->getCoin(index2) == walletsModel_->getCoin(index);
            }

            if(exist)
                walletsModelPerc_->addPercValue(index2-1, walletsModel_->getPortfolioPercentage(index));
            else
                walletsModelPerc_->add(new WalletPercItem {walletsModel_->getCoin(index),
                                        walletsModel_->getPortfolioPercentage(index),
                                        "",
                                        coinsModel_->getColor(walletsModel_->getCoin(index))});
        }
    }
    walletsModelPerc_->orderBy(WalletsPercModel::Order::ASC);

}

void BrokerManager::setCoinPtrInWallets()
{
    for(auto w : walletsModel_->wallets())
    {
        auto coin = findCoin(w->getCoin());
        if(coin != std::nullopt)
            w->setCoin(coin.value());
    }
    walletsModel_->updateLayout();
}

std::optional<std::vector<WalletOperation*>>  BrokerManager::getAvailableBalancesOrdered(const QString& coinID, const QString exchange)
{
    auto wallets = SQLManager::GetInstance()->getWalletsOps(BrokerManager::userID, coinID, exchange);
    if (wallets == std::nullopt)
        return std::nullopt;

    std::sort(wallets->begin(), wallets->end(), [&](WalletOperation* w1, WalletOperation* w2){
            return w1->getDate() < w2->getDate();
    });
    return wallets;
}

double  BrokerManager::getAvailableAmounts(const std::vector<WalletOperation*>& wallets) const
{
    double totalAvailableAmt = 0.0;
    for (auto w : wallets)
        totalAvailableAmt += w->getAvailable();
    return totalAvailableAmt;
}

bool BrokerManager::checkDuplicity(const QString& exchange, std::shared_ptr<Operation> operation)
{
    if(setWallets(exchange, operation) != static_cast<int>(NewOperationRes::OK)) return true; // Return as duplicated

    auto res = SQLManager::GetInstance()->getOperations(QString::number(operation->getWalletID1()));
    if(std::get<0>(res) ==  false) return false;
    
    auto ops = std::get<1>(res);
    auto exist = std::find_if(ops.begin(), ops.end(),
                              [&](Operation* op){return *op == *operation.get();});
    if(exist != ops.end())
    {
        qDebug() << "Operation is already in database (duplicated)";
        return true; // Return duplicated
    }
    return false;
}

std::optional<std::vector<Wallet*>> BrokerManager::findWallets(const QString& coin)
{
    return walletsModelAll_->find(coin);
}

std::optional<Wallet> BrokerManager::findWallet(const QString& exchange, const QString& coin)
{
    return SQLManager::GetInstance()->getWallets(BrokerManager::userID, coin, exchange); //walletsModelAll_->find(exchange, coin);
}

std::optional<Coin*> BrokerManager::findCoin(const QString& coin)
{

    auto res = SQLManager::GetInstance()->getCoin(coin);

    if(res == std::nullopt)
        return std::nullopt;
    else
        return new Coin(std::get<0>(res.value()), std::get<1>(res.value()), std::get<2>(res.value()), std::get<3>(res.value()));
}

void BrokerManager::updateCurrentPrice(void)
{
    auto coingecko = Coingecko::getInstance();

    auto prices = coingecko->getCurrentPrices();
    if(prices == std::nullopt)
    {
        std::cout << "Error: Prices information not provided" << std::endl;
        return;
    }

    for (auto wallet : walletsModel_->wallets())
    {
        auto coin = findCoin(wallet->getCoin());
        if(coin != std::nullopt && prices->find(coin.value()->name().toLower()) != prices->end())
            coin.value()->setCurrentPrice(prices->at(coin.value()->name().toLower()));
    }
    FINISHED = true;

    for (auto wallet : walletsModel_->wallets())
    {
        auto coin = findCoin(wallet->getCoin());
        if(coin != std::nullopt && coin.value()->currentPrice() < 0.0)
        {
            // Buscar la moneda individualmente
            auto price = getCurrentPrice(coin.value());
            if(price != std::nullopt)
                coin.value()->setCurrentPrice(price.value());
        }
    }

    for(auto c : coinsModel_->coins())
    {
        LOG_DEBUG("Coin %s Current Price %lf", qPrintable(c->name()), c->currentPrice());
    }
}

std::optional<double>  BrokerManager::getCurrentPrice(Coin* coin)
{
    auto coingecko = Coingecko::getInstance();
    auto c = coingecko->getCoinID("Binance", qPrintable(coin->name().toLower()));
    if(c == std::nullopt)
        return std::nullopt;
    return coingecko->getCurrentPrice(c.value());
}

BrokerManager::LoadResCode BrokerManager::load(void)
{
    UsersModel::setUsers();
    
    if(UsersModel::getUsers().size() == 0)
    {
        qDebug() << "[BrokerManager::load] No users in database. Please, create one before continue.";
        return LoadResCode::NO_USERS;
    }
    else
    {
    
        userID = std::get<0> (UsersModel::getUsers()[0]);
        year_ = 2022;

        std::cout << "usr ID: " << userID << std::endl;

        loadOperationsFromDB(userID);
        loadCoinsFromDB();
        loadExchangesFromDB();
        loadAssetTypesFromDB();
        loadWalletsFromDB(userID);
        //updateCurrentPrice(); //TODO: Run using threads after open app
        loadDepositsFromDB(userID);

        LOG_DEBUG("Loaded");
        return LoadResCode::OK;
    }

    return LoadResCode::UNKNOWN;
        

}

Operation* BrokerManager::getLastOperation(void) const
{
    return SQLManager::GetInstance()->getLastOperation();
}

std::vector<WalletOperation*> BrokerManager::getLastNWalletOperation(int limit) const
{
    return SQLManager::GetInstance()->getLastNWalletOperation(limit);
}


int BrokerManager::setWallets(const QString& exchange, std::shared_ptr<Operation> operation)
{

    auto res1 = findCoin(operation->getPair1());
    auto res2 = findCoin(operation->getPair2());
    if(res1 == std::nullopt) return static_cast<int>(NewOperationRes::ORI_COIN_NOT_EXIST);
    if(res1 == std::nullopt) return static_cast<int>(NewOperationRes::DEST_COIN_NOT_EXIST);
    
    auto walletPair1 = findWallet(exchange, QString::number(res1.value()->id()));
    auto walletPair2 = findWallet(exchange, QString::number(res2.value()->id()));

    if(walletPair1 == std::nullopt) return static_cast<int>(NewOperationRes::ORI_WALLET_NOT_EXIST);
    if(walletPair2 == std::nullopt) return static_cast<int>(NewOperationRes::DEST_WALLET_NOT_EXIST);

    operation->setWalletID1(walletPair1->getWalletID());
    operation->setWalletID2(walletPair2->getWalletID());

    return static_cast<int>(NewOperationRes::OK);
}



// Slots from filter bar C++ class

void BrokerManager::updateAssetTypeModel(const QString& category)
{
    LOG_DEBUG("");

    filterAssetTypes(category);
}

