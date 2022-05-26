//
// Created by gabridc on 26/11/21.
//

#include "BrokerManager.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "UsersModel.h"

uint32_t BrokerManager::userID = 0U;

BrokerManager::BrokerManager(const QObject* parent, OperationsModel*const operationsModel, WalletsModel*const walletsModel, WalletsModel*const walletsModelAll, WalletsPercModel*const walletsPercModel, CoinsModel*const coinsModel)
{
    parent = 0;
    operationsModel_ = operationsModel;
    walletsModel_ = walletsModel;
    walletsModelAll_ = walletsModelAll;
    walletsModelPerc_ = walletsPercModel;
    coinsModel_ = coinsModel;

    userID = std::get<0> (UsersModel::getUsers()[0]);

    std::cout << "usr ID: " << userID << std::endl;

    loadOperationsFromDB(userID);
    loadCoinsFromDB();
    loadWalletsFromDB(userID);
    loadDepositsFromDB(userID);

}

bool BrokerManager::newDeposit(const int walletID, double amount, double fees,
                const QString comment, QString date)
{
    if(date == "")
        date = QDateTime::currentDateTime().toString();

    auto res = DBLocal::GetInstance()->depositOperation(walletID, amount, 1, fees, comment, date);

    loadWalletsFromDB(userID);
    if(res)
        emit depositCompleted();
    return res;
}


bool BrokerManager::newOperation(const int walletID1, const int walletID2, double pair1Amount, double pair1AmountFiat,
                  double pair2Amount, double pair2AmountFiat, QString feesCoin, double comision, double comisionFiat, QString comments, QString type,
                  QString status, QString date)
{
    if(date == "")
        date = QDateTime::currentDateTime().toString();
    if(status == "")
        status = "Confirmed";

    double totalAmount = 0.0;

    //std::cout << type.toStdString() << date.toStdString() << std::endl;
    //std::cout << walletID1<< walletID2 << std::endl;

    auto [r1, wallet1] = DBLocal::GetInstance()->getWallet(walletID1);
    auto [r2, wallet2] = DBLocal::GetInstance()->getWallet(walletID2);


    if(r1 and r2)
    {
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

        //Se comprueba que haya saldo suficiente para la compra antes de proceder
        if(wallet1->getAmount() >= totalAmount)
        {

            auto res = DBLocal::GetInstance()->registerOperation(wallet1->getWalletID(), walletID2, totalAmount, pair1AmountFiat, pair2Amount, pair2AmountFiat,
                                                             feesCoin, comision, comisionFiat, comments, type, status, date);

            return res;
        }
        else
        {
            std::cout << "la wallet origen no tiene saldo suficiente para realizar la operación" << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "La wallet origen no existe." << std::endl;
        return false;
    }
}

bool BrokerManager::newOperation(const QString& exchange, std::shared_ptr<Operation> operation)
{
    auto ops = operationsModel_->operations();
    auto exist = std::find_if(ops.begin(), ops.end(),
                    [&](Operation* op){return *op == *operation.get();});
    if(exist != ops.end())
    {
        std::cout << "La operacion con fecha " +  operation->getDate().toStdString() + " ya existe en la base de datos" << std::endl;
        return false;
    }

    auto walletPair1 = findWallet(exchange, operation->getPair1());
    auto walletPair2 = findWallet(exchange, operation->getPair2());

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
                 operation->getDate());
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
        return (DBLocal::GetInstance()->addWallet(coinName, 0.0, exchange, userID) > 0);
    }

}

bool BrokerManager::importPreviewOperations(const QString& csvFilePath, const QString& type)
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
    //walletsModel_->clear();
    //operationsModel_->clear();
    //loadWalletsFromDB(userID, year);
    //loadOperationsFromDB(userID, year);
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

void BrokerManager::loadCoinsFromDB(void)
{
    auto coins = DBLocal::GetInstance()->getCoins();
    for(auto c : coins)
        coinsModel_->add(new Coin(std::get<0>(c), std::get<1>(c), std::get<2>(c)));

    updateCurrentPrice();

    for (auto wallet : walletsModel_->wallets())
    {
            auto coin = findCoin(wallet->getCoin());
            if(coin->currentPrice() < 0.0)
            {
                std::cout << "gadom " << coin->name().toStdString() << std::endl;
                // Buscar la moneda individualmente
                auto price = getCurrentPrice(coin);
                if(price != std::nullopt)
                    coin->setCurrentPrice(price.value());
            }
    }

    for(auto c : coinsModel_->coins())
    {
        std::cout << "Coin " << c->name().toStdString() << " Cur.Price: " << c->currentPrice() << std::endl;
    }
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
            std::cout << "Wallet: " <<  w->getWalletID() << std::endl;
            std::cout << "  Coin: " <<  w->getCoin().toStdString() << std::endl;
            std::cout << "  User: " <<  w->getUser().toStdString() << std::endl;
            std::cout << "  Cantidad de monedas: " <<  w->getAmount()  << std::endl;
            std::cout << "  Invertido: " <<  w->getInvested()  << std::endl;
            std::cout << "  Average Cost: " <<  w->getAverageCost() << std::endl;
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
    auto [r,deposits] = DBLocal::GetInstance()->getDeposits(userID);

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
        if(coin)
            w->setCoin(coin);
    }

}

std::optional<std::vector<Wallet*>> BrokerManager::findWallets(const QString& coin)
{
    return walletsModelAll_->find(coin);
}

std::optional<Wallet> BrokerManager::findWallet(const QString& exchange, const QString& coin)
{
    return walletsModelAll_->find(exchange, coin);
}

Coin* BrokerManager::findCoin(const QString& coin)
{

    auto coins = coinsModel_->coins();
    auto it = std::find_if (coins.begin(), coins.end(), [&](Coin* c){
        return c->name() == coin;
    });

    if(it != coins.end())
        return *it;
    else
        return nullptr;
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
        if(prices->find(coin->name().toLower()) != prices->end())
            coin->setCurrentPrice(prices->at(coin->name().toLower()));
    }
    FINISHED = true;
}

std::optional<double>  BrokerManager::getCurrentPrice(Coin* coin)
{
    Coingecko coingecko;
    auto c = coingecko.getCoinID("Binance", coin->name().toLower());
    if(c == std::nullopt)
        return std::nullopt;
    return coingecko.getCurrentPrice(c.value());
}



