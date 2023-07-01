//
// Created by gabridc on 18/11/21.
//

#ifndef BROKER_WALLETSMODEL_H
#define BROKER_WALLETSMODEL_H
#include <QAbstractListModel>
#include <QList>
#include <memory>
#include "Wallet.h"


class WalletsModel : public QAbstractListModel{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QList<Wallet* > wallets READ wallets)
public:
    enum RoleNames {
        WalletID = 1,
        Coin ,
        Exchange,
        User,
        Amount,
        Invested,
        Average,
        GlobalAverage,
        FiatCoin,
        PortfolioPercentage,
        DisplayText,
        CurrentPrice
    };

    enum class Order {
        ASC = 0,
        DESC,
        FIAT_FIRST,
        CRYPTO_FIRST
    };

    enum class Attribute {
        TYPE = 0,
        PORTFOLIO,
        INVESTED,
        EARNINGS
    };

    enum class AssetType {
        ALL = 0,
        CRYPTO,
        DEBT,
        ETF,
        FIAT,
        FUND,
        REAL_STATE,
        SHARE
    };    

    WalletsModel(QAbstractListModel* parent = 0){};
    //Expone el nombre de los atributos y los relaciona entre QML y C++
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //Permite leer los atributos de la clase Operation en QML
    QVariant data(const QModelIndex &index, int role) const;
    void add(Wallet* wallet);
    void clear(void);
    int count() const;
    void updateLayout(void){emit layoutChanged();};

    void orderBy(Attribute atr,  Order o) noexcept;
    std::optional<std::vector<Wallet*>> find(const QString& coin) const noexcept;
    std::optional<Wallet> find(const QString& exchange,  const QString& coin) noexcept;

signals:
    void countChanged();


public slots:
    double getTotalInvested(void) const;
    double getTotalInvested2(void) const;
    double getNonCryptoInvested(void) const;
    double getCryptoInvested(void) const;
    double calculatePortfolioPercentage(const double amount) const;
    double calculateGlobalAverageCost(const QString& coin) const;
    double getPortfolioPercentage(const int index) const;
    int getWalletID(const int index) const;
    QString getCoin(const int index) const;
    //void update(const int userID);
    QList<Wallet*> wallets(void);

private:
    QList<Wallet* > wallets_;
};


#endif //BROKER_WALLETSMODEL_H
