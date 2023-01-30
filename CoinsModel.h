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
    Q_PROPERTY(QString color READ color)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(double currentPrice READ currentPrice WRITE setCurrentPrice)
public:
    Coin(void) = default;
    Coin(int id, const QString& name, const QString& color, const QString& type) :
        id_(id),
        name_(name),
        color_(color),
        currentPrice_(-0.1),
        type_(type){};

    int id(void) const noexcept {return id_;};
    QString name(void) const noexcept {return name_;};
    QString color(void) const noexcept {return color_;};
    QString type(void) const noexcept {return type_;};
    double currentPrice(void) const noexcept {return currentPrice_;};

    void setCurrentPrice(const double price) noexcept {currentPrice_ = price;};

private:
    int id_;
    QString name_;
    QString color_;
    QString type_;
    double currentPrice_;
};


class CoinsModel : public QAbstractListModel{

    Q_OBJECT
    Q_PROPERTY(QList<Coin* > coins READ coins)
public:
    enum RoleNames {
        CoinID = 1,
        CoinName,
        CurrentPrice
    };

    CoinsModel(QObject* parent = 0){};

    //Expone el nombre de los atributos y los relaciona entre QML y C++
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //Permite leer los atributos de la clase Operation en QML
    QVariant data(const QModelIndex &index, int role) const;

    QList<Coin*> coins(void);
    void add(Coin* coin);
    QString getColor(const QString& name) const;
    void clear(void) noexcept;
public slots:
    int getID(const int index);

private:
    QList<Coin* > coins_;

};


#endif //BROKER_WALLETSMODEL_H
