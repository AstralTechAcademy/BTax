//
// Created by gabridc on 18/11/21.
//

#include "WalletsModel.h"

//Expone el nombre de los atributos y los relaciona entre QML y C++
QHash<int, QByteArray> WalletsModel::roleNames() const{return QHash<int, QByteArray> ();}
int WalletsModel::rowCount(const QModelIndex &parent) const{return 0;}

//Permite leer los atributos de la clase Operation en QML
QVariant WalletsModel::data(const QModelIndex &index, int role) const{return QVariant();}
void WalletsModel::add(Wallet* wallet){}


QList<Wallet*> WalletsModel::wallets(void){return wallets_;}
