//
// Created by gabridc on 9/11/21.
//

#include "WalletOperation.h"
#include "Logger.h"
#include <iostream>

void WalletOperation::print() const noexcept
{
    LOG_DEBUG("Wallet: %d"
                "  Operation ID: %d",
                "  Coin: %s",
                "  User: %s",
                "  Amount: %d",
                "  Retired: %d",
                "  Available: %d", 
                "  Fiat: %d", 
                "  Date: %s", 
                "  DateUTC: %s", 
                walletID_, 
                id_,
                qPrintable(coinName_),
                qPrintable(user_),
                amount_,
                retired_,
                available_,
                fiatPrice_,
                qPrintable(date_.toString()),
                qPrintable(datetimeUTC_.toString()));
}

void WalletOperation::print2() const noexcept
{
    LOG_DEBUG(  "      Retired:  %d"
                "      Available: %d",
                "      Fiat: %d",
                "      Date: %s",
            retired_,
            available_,
            fiatPrice_,
            qPrintable(date_.toString()));
}

double WalletOperation::getAmount(void) const noexcept {return amount_;}
int WalletOperation::getWalletID(void) const noexcept{return walletID_;};
int WalletOperation::getID(void) const noexcept{return id_;};
QDateTime WalletOperation::getDate(void) const noexcept{return date_;};
QDateTime WalletOperation::getDateTimeUtc(void) const noexcept{return datetimeUTC_;};
double WalletOperation::getRetired(void) const noexcept {return retired_;}
double WalletOperation::getAvailable(void) const noexcept {return available_;}
QString WalletOperation::getUser(void) const noexcept {return user_;};
QString WalletOperation::getExchange(void) const noexcept {return exchange_;};
QString WalletOperation::getCoin(void) const noexcept {return coinName_;}
double WalletOperation::getFiatPrice(void) const noexcept {return fiatPrice_;}
