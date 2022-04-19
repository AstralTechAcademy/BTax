//
// Created by gabridc on 9/11/21.
//

#include "Wallet.h"
#include <iostream>
#include "DBLocal.h"

void Wallet::print() const noexcept
{
    std::cout << getWalletID() << getCoin().toStdString() << getExchange().toStdString() << std::endl;
}


double Wallet::getAverageCost(void) const noexcept
{
    if(invested_ > 0.0000000 && amount_ > 0.0000000)
        return invested_ / amount_;
    else
        return 0.0000000;
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

void Wallet::setCoin(const QString coin)
{
    coin_ = coin;
}
void Wallet::setUser(const QString user)
{
    user_ = user;
}

void Wallet::setExchange(const QString exchange)
{
    exchange_ = exchange;
}

void Wallet::setWalletID(const int walletID)
{
    walletID_ =walletID;
}
