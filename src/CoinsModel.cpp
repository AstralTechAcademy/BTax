//
// Created by gabridc on 18/11/21.
//

#include "CoinsModel.h"
#include <iostream>

//Expone el nombre de los atributos y los relaciona entre QML y C++
QHash<int, QByteArray> CoinsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CoinID] = "coinID";
    roles[CoinName] = "name";
    roles[CurrentPrice] = "currentPrice";
    return roles;
}

int CoinsModel::rowCount(const QModelIndex &parent) const{return coins_.size();}

//Permite leer los atributos de la clase Operation en QML
QVariant CoinsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= coins_.count()) {
        return QVariant();
    }

    switch (role) {
        case CoinID:
            return coins_.at(row)->id();
        case CoinName:
            return QString::number(coins_.at(row)->id()) + " " + coins_.at(row)->name();
        case CurrentPrice:
            return coins_.at(row)->currentPrice();
        default:
            return coins_.at(row)->name();
    }

}

QList<Coin *> CoinsModel::coins(void) {
    return coins_;
}

void CoinsModel::add(Coin *coin) {
    coins_.push_back(coin);
}

int CoinsModel::getID(const int index)
{
    return (*coins_.at(index)).id();
}

QString CoinsModel::getColor(const QString& name) const
{
    for(auto i = 0; i < coins_.size(); i++)
    {

        if(coins_.at(i)->name() ==  name)
        {
            return coins_.at(i)->color();
        }

    }

    return "black";
}

void CoinsModel::clear(void) noexcept
{
    coins_ .clear();
}