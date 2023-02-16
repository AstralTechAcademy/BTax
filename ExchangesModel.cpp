//
// Created by gabridc on 18/11/21.
//

#include "ExchangesModel.h"
#include <iostream>

//Expone el nombre de los atributos y los relaciona entre QML y C++
QHash<int, QByteArray> ExchangesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ExchangeID] = "exchangeID";
    roles[ExchangeName] = "name";
    roles[TimeZone] = "timezone";
    return roles;
}

int ExchangesModel::rowCount(const QModelIndex &parent) const{return exchanges_.size();}

//Permite leer los atributos de la clase Operation en QML
QVariant ExchangesModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= exchanges_.count()) {
        return QVariant();
    }

    switch (role) {
        case ExchangeID:
            return exchanges_.at(row)->id();
        case ExchangeName:
            return QString::number(exchanges_.at(row)->id()) + " " + exchanges_.at(row)->name();
        case TimeZone:
            return exchanges_.at(row)->timezone();
        default:
            return exchanges_.at(row)->name();
    }

}

QList<Exchange *> ExchangesModel::exchanges(void) {
    return exchanges_;
}

void ExchangesModel::add(Exchange *exch) {
    exchanges_.push_back(exch);
    qDebug() << "add";
}

int ExchangesModel::getID(const int index)
{
    return (*exchanges_.at(index)).id();
}

QString ExchangesModel::getTimeZone(const QString& name) const
{
    for(auto i = 0; i < exchanges_.size(); i++)
    {

        if(exchanges_.at(i)->name() ==  name)
        {
            return exchanges_.at(i)->timezone();
        }

    }

    return "GMT";
}

void ExchangesModel::clear(void) noexcept
{
    exchanges_ .clear();
}