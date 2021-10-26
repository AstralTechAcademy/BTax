//
// Created by gabridc on 24/10/21.
//

#include "OperationsModel.h"



QHash<int, QByteArray> OperationsModel::roleNames() const {
QHash<int, QByteArray> roles;

roles[id] = "id";
roles[pair1] = "pair1";
roles[pair2] = "pair2";
roles[buyPrice] = "buyPrice";
roles[sellPrice] = "sellPrice";
roles[sellPrice] = "sellPrice";
roles[buyPriceFiat] = "buyPriceFiat";
roles[sellPriceFiat] = "sellPriceFiat";
roles[deposit] = "deposit";
roles[retired] = "retired";
roles[statusBuy] = "statusBuy";
roles[statusSell] = "statusSell";
roles[buyDate] = "buyDate";
roles[sellDate] = "sellDate";
roles[comments] = "comments";
return roles;
}


int OperationsModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
        return 0;

    return operations_.count();
}

QVariant OperationsModel::data(const QModelIndex &index, int role) const
{
QVariant value;

if (index.isValid()) {
    switch(role)
    {
        case Qt::UserRole + 1:
            value = operations_.at(index.row())->getID();
            break;
        case Qt::UserRole + 2:
            value = operations_.at(index.row())->getPair1();
            break;
        case Qt::UserRole + 3:
            value = operations_.at(index.row())->getPair2();
            break;
        case Qt::UserRole + 4:
            value = operations_.at(index.row())->getBuyPrice();
            break;
        case Qt::UserRole + 5:
            value = operations_.at(index.row())->getSellPrice();
            break;
        case Qt::UserRole + 6:
            value = operations_.at(index.row())->getBuyPriceFiat();
            break;
        case Qt::UserRole + 7:
            value = operations_.at(index.row())->getSellPriceFiat();
            break;
        case Qt::UserRole + 8:
            value = operations_.at(index.row())->getDeposit();
            break;
        case Qt::UserRole + 9:
            value = operations_.at(index.row())->getRetired();
            break;
        case Qt::UserRole + 10:
            value = operations_.at(index.row())->getStatusBuy();
            break;
        case Qt::UserRole + 11:
            value = operations_.at(index.row())->getStatusSell();
            break;
        case Qt::UserRole + 12:
            value = operations_.at(index.row())->getBuyDate();
            break;
        case Qt::UserRole + 13:
            value = operations_.at(index.row())->getSellDate();
            break;
        case Qt::UserRole + 14:
            value = operations_.at(index.row())->getComments();
            break;
        default:
            value = QVariant();
            break;

    }
}
return value;
}

void OperationsModel::add(Operation* operation)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());   // kindly provided by superclass
    operations_.push_back(operation);
    endInsertRows();                                          // kindly provided by superclass
}

QList<Operation*> OperationsModel::operations(void)
{
    return operations_;
}
void OperationsModel::setOperations(QList<Operation*> operations)
{
    operations_ = operations;
}