//
// Created by gabridc on 18/11/21.
//

#include "AssetTypeModel.h"
#include <iostream>

//Expone el nombre de los atributos y los relaciona entre QML y C++
QHash<int, QByteArray> AssetTypeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AssetTypeID] = "assetTypeID";
    roles[AssetTypeName] = "name";
    return roles;
}

int AssetTypeModel::rowCount(const QModelIndex &parent) const{return assets_.size();}

//Permite leer los atributos de la clase Operation en QML
QVariant AssetTypeModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= assets_.count()) {
        return QVariant();
    }

    switch (role) {
        case AssetTypeID:
            return assets_.at(row)->id();
        case AssetTypeName:
            return assets_.at(row)->name();
    }

}

QList<AssetType *> AssetTypeModel::assetTypes(void) {
    return assets_;
}

void AssetTypeModel::add(AssetType *asset) {
    assets_.push_back(asset);
}

void AssetTypeModel::clear(void)
{
    assets_.clear();
}

int AssetTypeModel::getID(const int index)
{
    return (*assets_.at(index)).id();
}
QString AssetTypeModel::getName(const int index)
{
    return (*assets_.at(index)).name();
}