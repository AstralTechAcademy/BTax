//
// Created by gabridc on 18/11/21.
//

#ifndef BROKER_WALLETSPERCMODEL_H
#define BROKER_WALLETSPERCMODEL_H
#include <QAbstractListModel>
#include <QList>
#include <tuple>


using WalletPercItem = std::tuple<QString, double, QString, QString>;

class WalletsPercModel : public QAbstractListModel{

    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum RoleNames {
        Coin ,
        PortfolioPercentage,
        Icon,
        Color,
    };

    enum class Order {
        ASC = 0,
        DESC
    };

    WalletsPercModel(QAbstractListModel* parent = 0){};

    //Expone el nombre de los atributos y los relaciona entre QML y C++
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //Permite leer los atributos de la clase Operation en QML
    QVariant data(const QModelIndex &index, int role) const;
    void add(WalletPercItem* wallet);
    void addPercValue(int index, double val);
    void clear(void);
    int count() const;

    void orderBy(Order o) noexcept;
    QString getCoin(const int index);

signals:
    void countChanged();

private:
    QList<WalletPercItem*> wallets_;

};


#endif //BROKER_WALLETSMODEL_H
