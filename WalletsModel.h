//
// Created by gabridc on 18/11/21.
//

#ifndef BROKER_WALLETSMODEL_H
#define BROKER_WALLETSMODEL_H
#include <QAbstractListModel>
#include <QList>
#include "Wallet.h"


class WalletsModel : public QAbstractListModel{

    Q_OBJECT
    Q_PROPERTY(QList<Wallet* > wallets READ wallets)
public:
    WalletsModel(QObject* parent = 0){};

    //Expone el nombre de los atributos y los relaciona entre QML y C++
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //Permite leer los atributos de la clase Operation en QML
    QVariant data(const QModelIndex &index, int role) const;
    void add(Wallet* wallet);

        QList<Wallet*> wallets(void);

private:
    QList<Wallet* > wallets_;

};


#endif //BROKER_WALLETSMODEL_H
