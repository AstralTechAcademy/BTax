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
    Wallet(const QString &coin, double amount, double invested, double available, double retired) :
    amount(amount),
    invested(invested),
    available(available),
    retired(retired),
    coin(coin) {}

    Wallet(const Wallet&) = default;
    Wallet(Wallet&&) = default;

    void print(void) const noexcept;

private:
    double amount;
    double invested;
    double available;
    double retired;
    QString coin;
};


#endif //BROKER_WALLET_H
