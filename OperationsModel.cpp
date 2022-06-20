//
// Created by gabridc on 24/10/21.
//

#include "OperationsModel.h"



QHash<int, QByteArray> OperationsModel::roleNames() const {
QHash<int, QByteArray> roles;

roles[id] = "id";
roles[pair1] = "pair1";
roles[pair2] = "pair2";
roles[pair1Amount] = "pair1Amount";
roles[pair1AmountFiat] = "pair1AmountFiat";
roles[pair2Amount] = "pair2Amount";
roles[pair2AmountFiat] = "pair2AmountFiat";
roles[comision] = "comision";
roles[comisionFiat] = "comisionFiat";
roles[status] = "status";
roles[date] = "date";
roles[comments] = "comments";
roles[type] = "type";
roles[ganancia] = "ganancia";
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
            value = operations_.at(index.row())->getPair1Amount();
            break;
        case Qt::UserRole + 5:
            value = operations_.at(index.row())->getPair1AmountFiat();
            break;
        case Qt::UserRole + 6:
            value = operations_.at(index.row())->getPair2Amount();
            break;
        case Qt::UserRole + 7:
            value = operations_.at(index.row())->getPair2AmountFiat();
            break;
        case Qt::UserRole + 8:
            value = operations_.at(index.row())->getComision();
            break;
        case Qt::UserRole + 9:
            value = operations_.at(index.row())->getComisionFiat();
            break;
        case Qt::UserRole + 10:
            value = operations_.at(index.row())->getStatus();
            break;
        case Qt::UserRole + 11:
            value = operations_.at(index.row())->getDate();
            break;
        case Qt::UserRole + 12:
            value = operations_.at(index.row())->getComments();
            break;
        case Qt::UserRole + 13:
            value = operations_.at(index.row())->getType();
            break;
        case Qt::UserRole + 14:
            value = operations_.at(index.row())->getGanancia();
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
    calculateEarnings();
}

void OperationsModel::clear(void)
{
    operations_.clear();
    totalEarnings_ = 0.0;
    emit dataChanged(index(0,0), index(rowCount(), 0));
    emit countChanged();
    emit layoutChanged();
}

QList<Operation*> OperationsModel::operations(void)
{
    return operations_;
}
void OperationsModel::setOperations(QList<Operation*> operations)
{
    operations_ = operations;
}

bool OperationsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid()) {
        switch(role)
        {
            case Qt::UserRole + 1:
                //operations_.at(index.row())->setID(value.toInt());
                break;
            case Qt::UserRole + 2:
                //operations_.at(index.row())->setPair1(value.toString());
                break;
            case Qt::UserRole + 3:
                //operations_.at(index.row())->setPair2(value.toString());
                break;
            case Qt::UserRole + 4:
                //operations_.at(index.row())->setPrice(value.toDouble());
                break;
            case Qt::UserRole + 5:
                break;
            case Qt::UserRole + 6:
                //operations_.at(index.row())->setDeposit(value.toString());
                break;
            case Qt::UserRole + 7:
                break;
            case Qt::UserRole + 8:
                break;
            case Qt::UserRole + 9:
                break;
            default:
                break;
        }
    }

    return true;
}

double OperationsModel::totalEarnings(void) const
{
    return totalEarnings_;
}

void OperationsModel::calculateEarnings(void)
{
    totalEarnings_ = 0.0;
    for(auto o : operations_)
        totalEarnings_ += o->getGanancia();
    emit totalEarningsChanged(totalEarnings_);
}