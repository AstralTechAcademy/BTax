//
// Created by gabridc on 9/11/21.
//

#include "Wallet.h"
#include <iostream>

void Wallet::print() const noexcept
{
    std::cout << "Wallet: " <<  walletID_ << std::endl;
    std::cout << "  Coin: " <<  coinName_.toStdString() << std::endl;
    std::cout << "  CoinID: " <<  coin_->id() << std::endl;
    std::cout << "  Type: " <<  coin_->type().toStdString() << std::endl;
    std::cout << "  User: " <<  user_.toStdString() << std::endl;
    std::cout << "  Cantidad de monedas: " <<  amount_  << std::endl;
    std::cout << "  Invertido: " <<  invested_  << std::endl;
    std::cout << "  Average Cost: " <<  avgPrice_ << std::endl;

    std::cout << "  Wallet Operations: " <<  std::endl;
    for(auto op : walletOps_)
        op->print2();
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

QString Wallet::getCoin(void) const noexcept {return coinName_;}
double Wallet::getCurPrice(void) const noexcept {
    return coin_ ? coin_->currentPrice() : 0.0;}
QString Wallet::getFiatCoin(void) const noexcept { return fiatCoin_;};
const Coin* Wallet::getpCoin(void) const noexcept { return coin_;}


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
    coinName_ = coin;
}
void Wallet::setUser(const QString user)
{
    user_ = user;
}
void Wallet::setCoin(Coin* coin)
{
    coin_ = coin;
}

void Wallet::setExchange(const QString exchange)
{
    exchange_ = exchange;
}

void Wallet::setWalletID(const int walletID)
{
    walletID_ =walletID;
}
