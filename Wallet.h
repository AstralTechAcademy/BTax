//
// Created by gabridc on 9/11/21.
//

#ifndef BROKER_WALLET_H
#define BROKER_WALLET_H
#include <QString>
#include <iostream>
#include "CoinsModel.h"
#include <memory>

class Wallet {

public:
    Wallet() = default;
    ~Wallet() = default;
    Wallet( int walletID, const QString &coin, const QString& exchange, const QString& user) :
            walletID_(walletID),
            coinName_(coin),
            exchange_(exchange),
            user_(user) {
        amount_ = 0.0;
        avgPrice_ = 0.0;
        fiatPrice_ = 0.0;
        invested_ = 0.0;
        coin_ = nullptr;
    };

    Wallet(const Wallet&) = default;
    Wallet(Wallet&&) = default;

    void print(void) const noexcept;
    double getAmount(void) const noexcept;
    int getWalletID(void) const noexcept;
    double getInvested(void) const noexcept;
    QString getUser(void) const noexcept;
    QString getExchange(void) const noexcept;
    QString getCoin(void) const noexcept;
    QString getFiatCoin(void) const noexcept;
    double getAverageCost(void) const noexcept;
    double getCurPrice(void) const noexcept;

    void setWalletID(const int walletID);
    void setInvested(const double invested);
    void setFiatPrice(const double average);
    void setAmount(const double average);
    void setExchange(const QString exchange);
    void setFiatCoin(const double average);
    void setCoin(const QString coin);
    void setUser(const QString user);
    void setCoin(Coin* coin);

    Coin* getpCoin(void) const noexcept;

private:
    double fiatPrice_;
    double avgPrice_;
    double amount_;
    QString coinName_;
    QString exchange_;
    QString fiatCoin_;
    QString user_;
    int walletID_;
    double invested_;
    Coin* coin_;
};


#endif //BROKER_WALLET_H
