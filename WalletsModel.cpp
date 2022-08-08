//
// Created by gabridc on 18/11/21.
//

#include "WalletsModel.h"
#include "BrokerManager.h"
#include "CoinsModel.h"

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
    roles[GlobalAverage] = "gAverage";
    roles[FiatCoin] = "fiatcoin";
    roles[PortfolioPercentage] = "portfolioPercentage";
    roles[CurrentPrice] = "currentPrice";
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
            return wallets_.at(row)->getWalletID();break;
        case Coin:
            return wallets_.at(row)->getCoin();break;
        case Exchange:
            return wallets_.at(row)->getExchange();break;
        case User:
            return wallets_.at(row)->getUser();break;
        case Amount:
            return wallets_.at(row)->getAmount();break;
        case Invested:
            return wallets_.at(row)->getInvested();break;
        case Average:
            return wallets_.at(row)->getAverageCost();break;
        case GlobalAverage:
            return calculateGlobalAverageCost(wallets_.at(row)->getCoin());break;
        case FiatCoin:
            return wallets_.at(row)->getFiatCoin();break;
        case PortfolioPercentage:
            return calculatePortfolioPercentage(wallets_.at(row)->getInvested());break;
        case DisplayText:
            return QString::number(wallets_.at(row)->getWalletID()) + " " + wallets_.at(row)->getUser() + " " + wallets_.at(row)->getCoin() + " "+wallets_.at(row)->getExchange();
            break;
        case CurrentPrice:
            return wallets_.at(row)->getCurPrice(); break;
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

double WalletsModel::getCryptoInvested(void) const
{
    double invested = 0.0 ;
    for(auto w : wallets_)
    {
        if(BrokerManager::getInstance() != nullptr)
        {
            auto c = BrokerManager::getInstance()->findCoin(w->getCoin());
            if(c != nullptr && c->type() != "fiat")
                invested += w->getInvested();
        }
    }

    return invested;
}


QList<Wallet*> WalletsModel::wallets(void){return wallets_;}

double WalletsModel::calculatePortfolioPercentage(const double invested) const
{
    return (invested * 100) / getTotalInvested();
}

double WalletsModel::getPortfolioPercentage(const int index) const
{
    return (wallets_.at(index)->getInvested() * 100) / getTotalInvested();
}

int WalletsModel::getWalletID(const int index) const
{
    return (*wallets_.at(index)).getWalletID();
}

QString WalletsModel::getCoin(const int index) const
{
    return (*wallets_.at(index)).getCoin();
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
                    return calculatePortfolioPercentage(w1->getInvested()) > calculatePortfolioPercentage(w2->getInvested());
                else
                    return calculatePortfolioPercentage(w1->getInvested()) < calculatePortfolioPercentage(w2->getInvested());
            });

            break;
        case Attribute::TYPE:
        {
            QList<Wallet* > walletsAux_;
            QList<Wallet* > walletsAux2_;

            // Copy only items that match the order filter
            std::copy_if(wallets_.begin(), wallets_.end(), std::back_inserter(walletsAux2_), [&](Wallet* w1){
                if ( o == Order::FIAT_FIRST )
                    return w1->getCoin() == "EUR" ;
                else return w1->getCoin() != "EUR" ;
            });
            walletsAux_ = walletsAux2_;
            walletsAux2_.clear();

            // Copy the rest of items that not match the filter
            std::copy_if(wallets_.begin(), wallets_.end(), std::back_inserter(walletsAux2_), [&](Wallet* w1){
                if ( o != Order::FIAT_FIRST )
                    return w1->getCoin() == "EUR" ;
                else return w1->getCoin() != "EUR" ;
            });

            // Add the rest of items behind the items that match the order filter
            for(auto a : walletsAux2_)
                walletsAux_.push_back(a);
            wallets_ = walletsAux_;
            walletsAux_.clear();
            walletsAux2_.clear();
        }
            break;
        default:
            return;
    }

    emit layoutChanged();
}

std::optional<std::vector<Wallet*>> WalletsModel::find(const QString& coin) const noexcept
{
    std::vector<Wallet*> wallets;

    auto it = wallets_.begin();
    while ((it = std::find_if(it, wallets_.end(), [&] (Wallet* w) { return w->getCoin() == coin; }))
           != wallets_.end())
    {
        wallets.push_back(wallets_[std::distance(wallets_.begin(), it)]);
        it++;
    }

    if(wallets.empty() == false)
        return wallets;
    else
        return std::nullopt;
}

std::optional<Wallet> WalletsModel::find(const QString& exchange,  const QString& coin) noexcept
{
    auto it = std::find_if(wallets_.begin(), wallets_.end(), [&](Wallet* w){
       if(w->getCoin() == coin && w->getExchange() == exchange)
           return true;
       else
           return false;

    });

    if(it != wallets_.end())
    {
        return **it;
    }
    else
        return std::nullopt;
}

double WalletsModel::calculateGlobalAverageCost(const QString& coin) const
{
    auto wallets = find(coin);
    auto average = 0.0;


    if(wallets == std::nullopt)
        return (double) average;

    for(auto w : wallets.value())
    {
        average += w->getAverageCost();
    }

    //std::cout << "File: WalletsModel Function: getGlobalAverageCost Description: Coin: " << coin.toStdString() << std::endl <<
      //          "       Total: " << average << "        Size: " << wallets.value().size() << "      Global Average: " << average / wallets.value().size() << std::endl;

    return average / wallets.value().size();

}
