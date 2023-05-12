//
// Created by gabridc on 18/11/21.
//

#ifndef BROKER_ASSETTYPEMODEL_H
#define BROKER_ASSETTYPEMODEL_H
#include <QAbstractListModel>
#include <QList>

class AssetType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString name READ name)
public:
    AssetType(void) = default;
    AssetType(int id, const QString& name) :
        id_(id),
        name_(name){};

    int id(void) const noexcept {return id_;};
    QString name(void) const noexcept {return name_;};

private:
    int id_;
    QString name_;
};


class AssetTypeModel : public QAbstractListModel{

    Q_OBJECT
    Q_PROPERTY(QList<AssetType* > assetTypes READ assetTypes)
public:
    enum RoleNames {
        AssetTypeID = 1,
        AssetTypeName
    };

    AssetTypeModel(QObject* parent = 0){};

    //Expone el nombre de los atributos y los relaciona entre QML y C++
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //Permite leer los atributos de la clase Operation en QML
    QVariant data(const QModelIndex &index, int role) const;

    QList<AssetType*> assetTypes(void);
    void add(AssetType* asset);
public slots:
    int getID(const int index);
    QString getName(const int index);


private:
    QList<AssetType* > assets_;

};


#endif //BROKER_WALLETSMODEL_H
