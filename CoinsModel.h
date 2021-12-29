//
// Created by gabridc on 18/11/21.
//

#ifndef BROKER_COINSMODEL_H
#define BROKER_COINSMODEL_H
#include <QAbstractListModel>
#include <QList>

class Coin : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString name READ name)
public:
    Coin(int id, const QString& name) :
        id_(id),
        name_(name){};

    int id(void) const noexcept {return id_;};
    QString name(void) const noexcept {return name_;};

private:
    int id_;
    QString name_;
};


class CoinsModel : public QAbstractListModel{

    Q_OBJECT
    Q_PROPERTY(QList<Coin* > coins READ coins)
public:
    enum RoleNames {
        CoinID = 1,
        CoinName
    };

    CoinsModel(QObject* parent = 0){};

    //Expone el nombre de los atributos y los relaciona entre QML y C++
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //Permite leer los atributos de la clase Operation en QML
    QVariant data(const QModelIndex &index, int role) const;

    QList<Coin*> coins(void);
    void add(Coin* coin);
public slots:
    int getID(const int index);

private:
    QList<Coin* > coins_;

};


#endif //BROKER_WALLETSMODEL_H
