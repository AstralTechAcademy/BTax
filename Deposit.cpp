//
// Created by gabridc on 10/11/21.
//

#include "Deposit.h"
#include <iostream>

void Deposit::print(void) const noexcept
{
    std::cout << amount << std::endl;
}
const QString& Deposit::getCoin() const {
    return coin;
}

double Deposit::getAmount() const {
    return amount;
}

double Deposit::getComision() const {
    return comision;
}

double Deposit::getAvailable() const {
    return available;
}

const QDateTime& Deposit::getDate() const {
    return date;
}

const QString& Deposit::getWalletID() const {
    return walletID;
}

const QString& Deposit::getUser() const {
    return user;
}
