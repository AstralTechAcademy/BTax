//
// Created by gabridc on 18/11/21.
//

#include "WalletsPercModel.h"

//Expone el nombre de los atributos y los relaciona entre QML y C++
QHash<int, QByteArray> WalletsPercModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Coin] = "coin";
    roles[PortfolioPercentage] = "portfolioPercentage";
    roles[Icon] = "icon";
    roles[Color] = "coinColor";
    return roles;
}
int WalletsPercModel::rowCount(const QModelIndex &parent) const{return wallets_.size();}

//Permite leer los atributos de la clase Operation en QML
QVariant WalletsPercModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= wallets_.count()) {
        return QVariant();
    }

    WalletPercItem* w = wallets_.at(row);
    switch (role) {
        case Coin:
            return std::get<Coin>(*w);
        case PortfolioPercentage:
            return std::get<PortfolioPercentage>(*w);
        case Icon:
            return std::get<Icon>(*w);
        case Color:
            return std::get<Color>(*w);
        default:
            return QVariant();
    }
}
void WalletsPercModel::add(std::tuple<QString, double, QString, QString>* wallet)
{
    wallets_.push_back(wallet);
    orderBy(Order::ASC);
    emit dataChanged(index(0,0), index(rowCount(), 0));
    emit countChanged();
    emit layoutChanged();


}

void WalletsPercModel::clear(void)
{
    wallets_.clear();
    emit dataChanged(index(0,0), index(rowCount(), 0));
    emit countChanged();
    emit layoutChanged();
}

int WalletsPercModel::count() const
{
    return rowCount();
}

void WalletsPercModel::orderBy(Order o) noexcept
{
    std::sort(wallets_.begin(), wallets_.end(), [&](WalletPercItem* w1, WalletPercItem* w2){
        if( o == Order::ASC )
            return std::get<PortfolioPercentage>(*w1) > std::get<PortfolioPercentage>(*w2);
        else
            return std::get<PortfolioPercentage>(*w1) < std::get<PortfolioPercentage>(*w2);
    });

    emit layoutChanged();
}

QString WalletsPercModel::getCoin(const int index)
{
    return std::get<Coin>(*wallets_.at(index));
}

void WalletsPercModel::addPercValue(int index, double val)
{
    std::get<PortfolioPercentage>(*wallets_.at(index)) += val;
}

