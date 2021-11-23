//
// Created by gabridc on 9/11/21.
//

#include "Wallet.h"
#include <iostream>
#include "DBLocal.h"

void Wallet::print() const noexcept
{
}


double Wallet::getAverageCost(void) const noexcept
{
    return invested_ / amount_;
}

double Wallet::getAmount(void) const noexcept {return amount_;}
int Wallet::getWalletID(void) const noexcept{return walletID_;};
double Wallet::getInvested(void) const noexcept {return invested_;}
QString Wallet::getUser(void) const noexcept {return user_;};
QString Wallet::getExchange(void) const noexcept {return exchange_;};

QString Wallet::getCoin(void) const noexcept {return coin_;}
QString Wallet::getFiatCoin(void) const noexcept { return fiatCoin_;};


void Wallet::setInvested(const double invested)
{
    invested_ = invested;
}
void Wallet::setFiatPrice(const double fiatPrice)
{
    fiatPrice_ = fiatPrice;
}
void Wallet::setAmount(const double amount)
{
    amount_ = amount;
}
void Wallet::setFiatCoin(const double fiatCoin)
{
    fiatCoin_ = fiatCoin;
}
