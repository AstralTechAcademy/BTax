//
// Created by gabridc on 26/11/21.
//

#include "BrokerManager.h"
#include "Broker.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "UsersModel.h"
#include "Validators/VOperation.h"


BrokerManager::BrokerManager(const QObject* parent, OperationsModel*const operationsModel, WalletsModel*const walletsModel, WalletsModel*const walletsModelAll, WalletsPercModel*const walletsPercModel, CoinsModel*const coinsModel, AssetTypeModel*const assetTypesModel)
{
    parent = 0;
    operationsModel_ = operationsModel;
    walletsModel_ = walletsModel;
    walletsModelAll_ = walletsModelAll;
    walletsModelPerc_ = walletsPercModel;
    coinsModel_ = coinsModel;
    assetTypesModel_ = assetTypesModel;
}

bool BrokerManager::newDeposit(const int walletID, double amount, double fees,
                const QString comment, QString date)
{
    QDateTime dateTime;
    dateTime.setDate(QDate(1900, 1, 1));
    if(date == "")
        date = QDateTime::currentDateTime().toString();
    else
    {

        dateTime.setDate(QDate(date.split(" ")[0].split("/")[2].toInt(),
                               date.split(" ")[0].split("/")[1].toInt(),
                               date.split(" ")[0].split("/")[0].toInt()));
        dateTime.setTime(QTime(date.split(" ")[1].split(":")[0].toInt(),
                               date.split(" ")[1].split(":")[1].toInt(),
                               date.split(" ")[1].split(":")[2].toInt()));
        date = dateTime.toString();
    }

    auto [r1, wallet1] = DBLocal::GetInstance()->getWallet(walletID);
    if(!r1)
        return false;

    auto res = DBLocal::GetInstance()->depositOperation(walletID, wallet1->getExchange() ,amount, 1, fees, comment, date);

    loadWalletsFromDB(userID);
    if(res)
        emit depositCompleted();
    return res;
}


int BrokerManager::newOperation(const int walletID1,const int walletID2, double pair1Amount, double pair1AmountFiat,
                                double pair2Amount, double pair2AmountFiat, QString feesCoin, double comision, double comisionFiat, QString comments, QString type,
                                QString status, QString date, std::vector<WalletOperation>& wOpsModified)
{
    qDebug() << "File: BrokerManager.cpp Function: newOperation";
    QDateTime dateTime;
    dateTime.setDate(QDate(1900, 1, 1));

    if(date == "")
        date = QDateTime::currentDateTime().toString();
    else
    {

        dateTime.setDate(QDate(date.split(" ")[0].split("/")[2].toInt(),
                                   date.split(" ")[0].split("/")[1].toInt(),
                                   date.split(" ")[0].split("/")[0].toInt()));
        dateTime.setTime(QTime(date.split(" ")[1].split(":")[0].toInt(),
                               date.split(" ")[1].split(":")[1].toInt(),
                               date.split(" ")[1].split(":")[2].toInt()));
        date = dateTime.toString();
        //dateTime.setTimeZone(QTimeZone::utc());
    }

    if(status == "")
        status = "Confirmed";

    double totalAmount = 0.0;

    auto [r1, wallet1] = DBLocal::GetInstance()->getWallet(walletID1);
    auto [r2, wallet2] = DBLocal::GetInstance()->getWallet(walletID2);

    if(r1 and r2)
    {
        auto coin = findCoin(wallet1->getCoin());
        if(coin == std::nullopt)
        {
            std::cout << "Coin origen no existe." << std::endl;
            return static_cast<int>( NewOperationRes::COIN_NOT_FOUND);
        }

        if(feesCoin == wallet1->getCoin())
        {
            //std::cout << "Same coin fees"<< std::endl;
            totalAmount = pair1Amount + comision;  // Formula reviwed. OK.
        }
        else
        {
            //std::cout << "Not same coin fees. Calculation to same coin: " << (comision * comisionFiat) / pair1AmountFiat << std::endl;
            totalAmount = pair1Amount + (comision * comisionFiat) / pair1AmountFiat; // Formula reviwed. OK.
        }

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

        VOperation voperation(data, dateTime);
        if( voperation.validate() == false)
            return static_cast<int>( NewOperationRes::VALIDATION_ERROR);

         if(coin.value()->type() == "fiat")
        {
            //Se comprueba que haya saldo suficiente para la compra antes de proceder
            auto ws = getAvailableBalancesOrdered(QString::number(coin.value()->id()), wallet1->getExchange());

            if (ws == std::nullopt)
            {
                std::cout << "No existen wallets de la moneda " << wallet1->getCoin().toStdString() << std::endl;
                return static_cast<int>( NewOperationRes::ORI_WALLET_NOT_EXIST);
            }

            auto wallets = ws.value();
            double totalAvailableAmt = getAvailableAmounts(wallets);

            if(totalAvailableAmt >= totalAmount)
            {
                auto res = DBLocal::GetInstance()->registerOperationNew(wallets, data, wOpsModified);
                return res ? static_cast<int>( NewOperationRes::ADDED) :  static_cast<int>( NewOperationRes::NOT_ADDED);
            }
            else
            {
                std::cout << "la wallet origen no tiene saldo suficiente para realizar la operación" << std::endl;
                return static_cast<int>( NewOperationRes::INSUF_BALANCE_ORI_WALLET);
            }
        }
        else
        {
            // Calculate Total Available amount
            auto ws = getAvailableBalancesOrdered(QString::number(coin.value()->id()));

            if (ws == std::nullopt)
            {
                std::cout << "No existen wallets de la moneda " << wallet1->getCoin().toStdString() << std::endl;
                return static_cast<int>( NewOperationRes::ORI_WALLET_NOT_EXIST);
            }
            auto wallets = ws.value();
            double totalAvailableAmt = getAvailableAmounts(wallets);

            if(totalAvailableAmt >= totalAmount)
            {
                auto res = DBLocal::GetInstance()->registerOperationNew(wallets, data, wOpsModified);
                return res ? static_cast<int>( NewOperationRes::ADDED) :  static_cast<int>( NewOperationRes::NOT_ADDED);
            }
            else
            {
                std::cout << "la wallet origen no tiene saldo suficiente para realizar la operación" << std::endl;
                return static_cast<int>( NewOperationRes::INSUF_BALANCE_ORI_WALLET);
            }
        }
    }
    else
    {
        std::cout << "La wallet origen no existe." << std::endl;
        if(!r1)
            return static_cast<int>( NewOperationRes::ORI_WALLET_NOT_EXIST);
        if(!r2)
            return static_cast<int>( NewOperationRes::DEST_WALLET_NOT_EXIST);
    }
}

int BrokerManager::newOperation(const QString& exchange, std::shared_ptr<Operation> operation, std::vector<WalletOperation>& wOpsModified)
{
    auto ops = operationsModel_->operations();

    if(isDuplicated(operation))
    {
        std::cout << "La operacion con fecha " +  operation->getDate().toStdString() + " ya existe en la base de datos" << std::endl;
        return static_cast<int>( NewOperationRes::ALREADY_ADDED);
    }

    auto walletPair1 = findWallet(exchange, operation->getPair1());
    auto walletPair2 = findWallet(exchange, operation->getPair2());

    qDebug() << "Pair1: " << walletPair1->getWalletID() << " " << operation->getPair1();
    return newOperation(walletPair1->getWalletID(),
                 walletPair2->getWalletID(),
                 operation->getPair1Amount(),
                 operation->getPair1AmountFiat(),
                 operation->getPair2Amount(),
                 operation->getPair2AmountFiat(),
                 operation->getFeesCoin(),
                 operation->getComision(),
                 operation->getComisionFiat(),
                 operation->getComments(),
                 operation->getType(),
                 operation->getStatus(),
                 operation->getDate(),
                 wOpsModified);
}

bool BrokerManager::newAsset(const QString& type, const QString& name, const QString& color)
{
    auto assets = coinsModel_->coins();
    auto exist = std::find_if(assets.begin(), assets.end(),
                              [&](Coin* coin){return coin->name() == name;});
    if(exist != assets.end())
    {
        std::cout << "El asset " +  name.toStdString() + " ya existe en la base de datos" << std::endl;
        return false;
    }

    return DBLocal::GetInstance()->registerAsset(type, name, color);
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
        return (DBLocal::GetInstance()->addWallet(coinName, exchange, userID) > 0);
    }

}

bool BrokerManager::importPreviewOperations(const QString& csvFilePath)
{
    std::cout << "Import Path: " << csvFilePath.toStdString() << std::endl;

    importPreview.clear();

    QFile file(csvFilePath.split("file://")[1]);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QStringList wordList;
  /*  while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString operation = line.split(';')[0];
        importPreview.push_back({});

        if(type == "Staking" && operation.size() == IMPORT_STAKING_OP_ATRS)
        {
            wordList.append();
        }

    }*/

    for(auto s : wordList)
        std::cout << s.toStdString() << std::endl;
}

bool BrokerManager::importOperations(void)
{
    //TODO: Grabr en DB las operaciones

    importPreview.clear();

    /*return DBLocal::GetInstance()->registerOperation(std::get<1>(w)->getWalletID(), walletID2, exchange, pair1, pair2, pair1Amount, pair1AmountFiat, pair2Amount, pair2AmountFiat,
                                                     comision, comisionFiat, comments, type, status, date);*/
}


uint32_t BrokerManager::getUserID(const QString& username)
{
    return DBLocal::GetInstance()->getUserID(username);
}


QStringList BrokerManager::getWalletsCBox(const QString& username)
{
    QStringList result;
    std::vector<Wallet *> wallets;
    if(username != "")
        wallets = std::get<1>(DBLocal::GetInstance()->getWallets(getUserID(username)));
    else
        wallets = std::get<1>(DBLocal::GetInstance()->getWallets(userID));

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
    userID = getUserID(username);
    std::cout << "user ID: " << userID << std::endl;
    walletsModel_->clear();
    operationsModel_->clear();
    loadWalletsFromDB(userID);
    loadOperationsFromDB(userID);
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
    auto operations = DBLocal::GetInstance()->getOperations(userID);
    if(std::get<0>(operations) == true)
    {
        for(auto op : std::get<1>(operations))
            operationsModel_->add(op);
    }
}

void BrokerManager::loadOperationsFromDB(const uint32_t userID, const uint32_t year)
{
    auto operations = DBLocal::GetInstance()->getOperations(userID);
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
    auto coins = DBLocal::GetInstance()->getCoins();
    for(auto c : coins)
        coinsModel_->add(new Coin(std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c)));
}

void BrokerManager::loadAssetTypesFromDB(void)
{
    auto assets = DBLocal::GetInstance()->getAssetTypes();
    for(auto c : assets)
        assetTypesModel_->add(new AssetType(std::get<0>(c), std::get<1>(c)));
}

void BrokerManager::loadWalletsFromDB(const uint32_t userID)
{
    walletsModel_->clear();
    walletsModelAll_->clear();
    auto result = DBLocal::GetInstance()->getWallets(userID);
    if(std::get<0>(result) == true)
    {
        auto wallets = std::get<1>(result);
        for(auto w : wallets)
        {
            //w->print();
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
    auto result = DBLocal::GetInstance()->getDeposits(userID);

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
    auto wallets = DBLocal::GetInstance()->getWalletsOps(BrokerManager::userID, coinID, exchange);
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
    qDebug() << "Total Available Amount = " << totalAvailableAmt;
    return totalAvailableAmt;
}

bool BrokerManager::isDuplicated(std::shared_ptr<Operation> operation)
{
    auto ops = operationsModel_->operations();
    auto exist = std::find_if(ops.begin(), ops.end(),
                              [&](Operation* op){return *op == *operation.get();});
    if(exist != ops.end())
    {
        std::cout << "La operacion con fecha " +  operation->getDate().toStdString() + " ya existe en la base de datos" << std::endl;
        return true;
    }
    return false;
}

std::optional<std::vector<Wallet*>> BrokerManager::findWallets(const QString& coin)
{
    return walletsModelAll_->find(coin);
}

std::optional<Wallet> BrokerManager::findWallet(const QString& exchange, const QString& coin)
{
    return DBLocal::GetInstance()->getWallets(BrokerManager::userID, coin, exchange); //walletsModelAll_->find(exchange, coin);
}

std::optional<Coin*> BrokerManager::findCoin(const QString& coin)
{

    auto res = DBLocal::GetInstance()->getCoin(coin);

    if(res == std::nullopt)
        return std::nullopt;
    else
        return new Coin(std::get<0>(res.value()), std::get<1>(res.value()), std::get<2>(res.value()), std::get<3>(res.value()));
}

void BrokerManager::updateCurrentPrice(void)
{
    Coingecko coingecko;

    auto prices = coingecko.getCurrentPrices();
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
            std::cout << "gadom " << coin.value()->name().toStdString() << std::endl;
            // Buscar la moneda individualmente
            auto price = getCurrentPrice(coin.value());
            if(price != std::nullopt)
                coin.value()->setCurrentPrice(price.value());
        }
    }

    for(auto c : coinsModel_->coins())
    {
        std::cout << "Coin " << c->name().toStdString() << " Cur.Price: " << c->currentPrice() << std::endl;
    }
}

std::optional<double>  BrokerManager::getCurrentPrice(Coin* coin)
{
    Coingecko coingecko;
    auto c = coingecko.getCoinID("Binance", coin->name().toLower());
    if(c == std::nullopt)
        return std::nullopt;
    return coingecko.getCurrentPrice(c.value());
}

void BrokerManager::load(void)
{
    UsersModel::setUsers();
    userID = std::get<0> (UsersModel::getUsers()[0]);
    year_ = 2022;

    std::cout << "usr ID: " << userID << std::endl;

    loadOperationsFromDB(userID);
    loadCoinsFromDB();
    loadAssetTypesFromDB();
    loadWalletsFromDB(userID);
    //updateCurrentPrice(); //TODO: Run using threads after open app
    loadDepositsFromDB(userID);

    std::cout << "Loaded" << std::endl;
}

Operation* BrokerManager::getLastOperation(void) const
{
    return DBLocal::GetInstance()->getLastOperation();
}

std::vector<WalletOperation*> BrokerManager::getLastNWalletOperation(int limit) const
{
    return DBLocal::GetInstance()->getLastNWalletOperation(limit);
}


