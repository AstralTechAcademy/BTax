//
// Created by gabridc on 18/11/21.
//

#include "WalletsModel.h"

//Expone el nombre de los atributos y los relaciona entre QML y C++
QHash<int, QByteArray> WalletsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WalletID] = "walletID";
    roles[Coin] = "coin";
    roles[Exchange] = "exchange";
    roles[User] = "user";
    roles[Amount] = "amount";
    roles[Invested] = "invested";
    roles[Average] = "average";
    roles[FiatCoin] = "fiatcoin";
    roles[PortfolioPercentage] = "portfolioPercentage";
    roles[DisplayText] = "display";
    return roles;
}
int WalletsModel::rowCount(const QModelIndex &parent) const{return wallets_.size();}

//Permite leer los atributos de la clase Operation en QML
QVariant WalletsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= wallets_.count()) {
        return QVariant();
    }

    switch (role) {
        case WalletID:
            return wallets_.at(row)->getWalletID();
        case Coin:
            return wallets_.at(row)->getCoin();
        case Exchange:
            return wallets_.at(row)->getExchange();
        case User:
            return wallets_.at(row)->getUser();
        case Amount:
            return wallets_.at(row)->getAmount();
        case Invested:
            return wallets_.at(row)->getInvested();
        case Average:
            return wallets_.at(row)->getAverageCost();
        case FiatCoin:
            return wallets_.at(row)->getFiatCoin();
        case PortfolioPercentage:
            return getPortfolioPercentage(wallets_.at(row)->getInvested());
        case DisplayText:
            return QString::number(wallets_.at(row)->getWalletID()) + " " + wallets_.at(row)->getUser() + " " + wallets_.at(row)->getCoin() + " "+wallets_.at(row)->getExchange();
        default:
            return QVariant();
    }
}
void WalletsModel::add(Wallet* wallet)
{
    wallets_.push_back(wallet);
    emit dataChanged(index(0,0), index(rowCount(), 0));
    emit countChanged();
    emit layoutChanged();


}

void WalletsModel::clear(void)
{
    wallets_.clear(),
    emit dataChanged(index(0,0), index(rowCount(), 0));
    emit countChanged();
    emit layoutChanged();
}

double WalletsModel::getTotalInvested(void) const
{
    double invested = 0.0 ;
    for(auto w : wallets_)
        invested += w->getInvested();

    return invested;
}


QList<Wallet*> WalletsModel::wallets(void){return wallets_;}

double WalletsModel::getPortfolioPercentage(const double invested) const
{
    return (invested * 100) / getTotalInvested();
}

int WalletsModel::getWalletID(const int index)
{
    return (*wallets_.at(index)).getWalletID();
}

int WalletsModel::count() const
{
    return rowCount();
}

void WalletsModel::orderBy(Attribute atr,  Order o) noexcept
{
    if (atr == Attribute::PORTFOLIO && ( o == Order::CRYPTO_FIRST || o == Order::FIAT_FIRST ) )
        return;
    if (atr == Attribute::TYPE && ( o == Order::ASC || o == Order::DESC ) )
        return;

    switch(atr)
    {
        case  Attribute::PORTFOLIO:
            std::sort(wallets_.begin(), wallets_.end(), [&](Wallet* w1, Wallet* w2){
                if( o == Order::ASC )
                    return getPortfolioPercentage(w1->getInvested()) > getPortfolioPercentage(w2->getInvested());
                else
                    return getPortfolioPercentage(w1->getInvested()) < getPortfolioPercentage(w2->getInvested());
            });

            break;
        case Attribute::TYPE:
            std::sort(wallets_.begin(), wallets_.end(), [&](Wallet* w1, Wallet* w2){
                if ( o == Order::FIAT_FIRST )
                    return w1->getCoin() == "EUR" ;
                else return w1->getCoin() != "EUR" ;
            });
            break;
        default:
            return;
    }

    emit layoutChanged();
}
