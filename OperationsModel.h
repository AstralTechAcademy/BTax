//
// Created by gabridc on 24/10/21.
//

#ifndef BROKER_OPERATIONSMODEL_H
#define BROKER_OPERATIONSMODEL_H
#include <QAbstractListModel>
#include "Operation.h"


class OperationsModel : public QAbstractListModel{

    Q_OBJECT
   Q_PROPERTY(QList<Operation*> operations READ operations WRITE setOperations)
public:
    enum OperationRoles {
        id = Qt::UserRole + 1,
        pair1,
        pair2,
        buyPrice,
        sellPrice,
        buyPriceFiat,
        sellPriceFiat,
        deposit,
        retired,
        statusBuy,
        statusSell,
        buyDate,
        sellDate,
        comments,
    };
    OperationsModel(QObject *parent = 0){
        add(new Operation(1, "ADA", "EUR", 100000, 10.0, 1.0, 10.0, 100, 0.0, "Not Confirmed", "Not Confirmed", "10/10/2021", "00/00/00", "Gabriel"));
        add(new Operation(2, "ADA", "EUR", 1.0, 10.0, 1.0, 10.0, 100, 0.0, "Not Confirmed", "Not Confirmed",  "10/10/2021", "00/00/00", "Gabriel"));
        add(new Operation(3, "ADA", "EUR", 1.0, 10.0, 1.0, 10.0, 100, 0.0, "Not Confirmed", "Not Confirmed", "10/10/2021", "00/00/00", "Gabriel"));
        add(new Operation(4, "ADA", "EUR", 1.0, 10.0, 1.0, 10.0, 100, 0.0, "Not Confirmed", "Not Confirmed", "10/10/2021", "00/00/00", "Gabriel"));
        add(new Operation(5, "ADA", "EUR", 1.0, 10.0, 1.0, 10.0, 100, 0.0, "Not Confirmed", "Not Confirmed", "10/10/2021", "00/00/00", "Gabriel"));
        add(new Operation(6, "ADA", "EUR", 1.0, 10.0, 1.0, 10.0, 100, 0.0, "Not Confirmed", "Not Confirmed", "10/10/2021", "00/00/00", "Gabriel"));
        add(new Operation(7, "ADA", "EUR", 1.0, 10.0, 1.0, 10.0, 100, 0.0, "Not Confirmed", "Not Confirmed", "10/10/2021", "00/00/00", "Gabriel"));

    };
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void add(Operation* operation);
    QList<Operation*> operations(void);
    void setOperations(QList<Operation*> operations);
private:
    QList<Operation*> operations_;

};


#endif //BROKER_OPERATIONSMODEL_H
