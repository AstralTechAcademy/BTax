//
// Created by gabridc on 26/11/21.
//

#include "BrokerManager.h"
#include "UsersModel.h"

uint32_t BrokerManager::userID = 0U;

BrokerManager::BrokerManager(const QObject* parent, OperationsModel*const operationsModel, WalletsModel*const walletsModel, WalletsModel*const walletsModelDeposit, CoinsModel*const coinsModel)
{
    parent = 0;
    operationsModel_ = operationsModel;
    walletsModel_ = walletsModel;
    walletsModelDeposit_ = walletsModelDeposit;
    coinsModel_ = coinsModel;

    userID = std::get<0> (UsersModel::getUsers()[0]);

    std::cout << "usr ID: " << userID << std::endl;

    loadOperationsFromDB();
    loadWalletsFromDB(userID);
    loadCoinsFromDB();

}

bool BrokerManager::newDeposit(const int walletID, double amount, double fees,
                const QString comment, QString date)
{
    if(date == "")
        date = QDateTime::currentDateTime().toString();

    return DBLocal::GetInstance()->depositOperation(walletID, amount, 1, fees, comment, date);
}


bool BrokerManager::newOperation(const int user, const QString exchange, QString pair1, QString pair2, double pair1Amount, double pair1AmountFiat,
                  double pair2Amount, double pair2AmountFiat, double comision, double comisionFiat, QString comments, QString type,
                  QString status, QString date)
{
    if(date == "")
        date = QDateTime::currentDateTime().toString();
    if(status == "")
        status = "Confirmed";

    std::cout << type.toStdString() << date.toStdString() << std::endl;

    auto w = DBLocal::GetInstance()->getWallet(user, exchange, pair1);
    auto walletID2 = DBLocal::GetInstance()->getWalletID(user, exchange, pair2);

    // Si no existe la wallet de PAIR2 se crea
    if(walletID2 == 0)
    {
        walletID2 = DBLocal::GetInstance()->addWallet(pair2, 0.0, exchange, user);
    }

    if(std::get<0>(w) == true)
    {
        std::cout << "Wallet Found "<< std::get<1>(w)->getWalletID() << " " << std::get<1>(w)->getAmount() << " " << pair1Amount  << std::endl;

        //Se comprueba que haya saldo suficiente para la compra antes de proceder
        if(std::get<1>(w)->getAmount() >= pair1Amount)
        {
            return DBLocal::GetInstance()->registerOperation(std::get<1>(w)->getWalletID(), walletID2, exchange, pair1, pair2, pair1Amount, pair1AmountFiat, pair2Amount, pair2AmountFiat,
                                                             comision, comisionFiat, comments, type, status, date);
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

bool BrokerManager::addWallet(const QString coinName, const QString exchange)
{
    if(walletsModel_->rowCount() > 0)
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
    else
        return  false;

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
            w->print();
            std::cout << w->getWalletID() <<  w->getCoin().toStdString() <<  w->getExchange().toStdString()<< std::endl;
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
    loadWalletsFromDB(userID);
}

void BrokerManager::loadOperationsFromDB(void)
{
    auto operations = DBLocal::GetInstance()->getOperations();
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
        coinsModel_->add(new Coin(std::get<0>(c), std::get<1>(c)));
}

void BrokerManager::loadWalletsFromDB(const uint32_t userID)
{
    auto result = DBLocal::GetInstance()->getWallets(userID);
    if(std::get<0>(result) == true)
    {
        auto wallets = std::get<1>(result);
        for(auto w : wallets)
        {
            std::cout << "Wallet: " <<  w->getCoin().toStdString() << std::endl;
            std::cout << "  User: " <<  w->getUser().toStdString() << std::endl;
            std::cout << "  Cantidad de monedas: " <<  w->getAmount()  << std::endl;
            std::cout << "  Invertido: " <<  w->getInvested()  << std::endl;
            std::cout << "  Average Cost: " <<  w->getAverageCost() << std::endl;
            //if(w->getAmount() > 0.0)
            walletsModel_->add(w);
            walletsModelDeposit_->add(w); // En el arranque se inicializan con los mismos datos
        }
    }
}

