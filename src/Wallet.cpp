//
// Created by gabridc on 9/11/21.
//

#include "Wallet.h"
#include "Logger.h"
#include <iostream>

void Wallet::print() const noexcept
{
    LOG_DEBUG("Wallet: %d"
                "  Coin: %d",
                "  CoinID: %s",
                "  Type: %s",
                "  User: %d",
                "  Amount: %d",
                "  Invested: %d", 
                "  Average Cost: %d",
                "  Wallet Operations: ",
                walletID_, 
                qPrintable(coinName_),
                coin_->id(),
                qPrintable(toString(coin_->type())),
                qPrintable(user_),
                amount_,
                invested_,
                avgPrice_);

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

const bool Wallet::isFiat(void) const noexcept
{
    return coin_->type() == EN_AssetType::FIAT;
}

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
void Wallet::setFiatCoin(const QString fiatCoin)
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
