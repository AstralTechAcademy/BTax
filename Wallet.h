//
// Created by gabridc on 9/11/21.
//

#ifndef BROKER_WALLET_H
#define BROKER_WALLET_H
#include <QString>

class Wallet {


public:
    Wallet() = delete;
    ~Wallet() = default;
    Wallet(const QString &coin, double amount, double invested, double retired, double available, const QString& symbol, double fiat, const QString& exchange, int walletID) :
    amount(amount),
    invested(invested),
    available(available),
    symbol(symbol),
    retired(retired),
    coin(coin),
    fiat(fiat),
    exchange(exchange),
    walletID(walletID){}

    Wallet(const Wallet&) = default;
    Wallet(Wallet&&) = default;

    void print(void) const noexcept;

private:
    double amount;
    double invested;
    double available;
    double retired;
    double fiat;
    QString coin;
    QString exchange;
    int walletID;
    QString symbol;
};


#endif //BROKER_WALLET_H
