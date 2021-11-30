//
// Created by gabridc on 26/11/21.
//

#include "BrokerManager.h"

BrokerManager::BrokerManager(const QObject* parent, OperationsModel*const operationsModel, WalletsModel*const walletsModel)
{
    parent = 0;
    operationsModel_ = operationsModel;
    walletsModel_ = walletsModel;

    loadOperationsFromDB();
    loadWalletsFromDB("Gabriel");

}
bool BrokerManager::newDeposit(const QString user, const QString exchange, const QString pair, double pairAmount, double fees,
                const QString comment, QString date)
{
    if(date == "")
        date = QDateTime::currentDateTime().toString();

    auto walletID = DBLocal::GetInstance()->getWalletID(user, exchange, pair);

    // Si no existe la wallet de PAIR2 se crea
    if(walletID == 0)
    {
        walletID = DBLocal::GetInstance()->addWallet(pair, 0.0, exchange, user);
    }

    return DBLocal::GetInstance()->depositOperation(walletID, pairAmount, 1, comment, date);


}


bool BrokerManager::newOperation(const QString user, const QString exchange, QString pair1, QString pair2, double pair1Amount, double pair1AmountFiat,
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

void BrokerManager::loadOperationsFromDB(void)
{
    auto operations = DBLocal::GetInstance()->getOperations();
    if(std::get<0>(operations) == true)
    {
        for(auto op : std::get<1>(operations))
            operationsModel_->add(op);
    }
}

void BrokerManager::loadWalletsFromDB(const QString& user)
{
    auto result = DBLocal::GetInstance()->getWallets(user);
    if(std::get<0>(result) == true)
    {
        auto wallets = std::get<1>(result);
        for(auto w : wallets)
        {
            std::cout << "Wallet: " <<  w->getCoin().toStdString() << std::endl;
            std::cout << "  Cantidad de monedas: " <<  w->getAmount()  << std::endl;
            std::cout << "  Invertido: " <<  w->getInvested()  << std::endl;
            std::cout << "  Average Cost: " <<  w->getAverageCost() << std::endl;
            if(w->getAmount() > 0.0)
                walletsModel_->add(w);
        }
    }
}