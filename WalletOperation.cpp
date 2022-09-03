//
// Created by gabridc on 9/11/21.
//

#include "WalletOperation.h"
#include <iostream>

void WalletOperation::print() const noexcept
{
    std::cout << "Wallet: " <<  walletID_ << std::endl;
    std::cout << "  Operation ID: " <<  id_ << std::endl;
    std::cout << "  Coin: " <<  coinName_.toStdString() << std::endl;
    std::cout << "  User: " <<  user_.toStdString() << std::endl;
    std::cout << "  Cantidad de monedas: " <<  amount_  << std::endl;
    std::cout << "  Retired: " <<  retired_  << std::endl;
    std::cout << "  Available: " <<  available_ << std::endl;
    std::cout << "  Fiat: " <<  fiatPrice_ << std::endl;
    std::cout << "  Date: " <<  date_.toString().toStdString() << std::endl;
}

void WalletOperation::print2() const noexcept
{
    std::cout << "      Retired: " <<  retired_  << " ";
    std::cout << "      Available: " <<  available_ << " ";
    std::cout << "      Fiat: " <<  fiatPrice_ << " ";
    std::cout << "      Date: " <<  date_.toString().toStdString() << std::endl;
}

double WalletOperation::getAmount(void) const noexcept {return amount_;}
int WalletOperation::getWalletID(void) const noexcept{return walletID_;};
int WalletOperation::getID(void) const noexcept{return id_;};
QDateTime WalletOperation::getDate(void) const noexcept{return date_;};
double WalletOperation::getRetired(void) const noexcept {return retired_;}
double WalletOperation::getAvailable(void) const noexcept {return available_;}
QString WalletOperation::getUser(void) const noexcept {return user_;};
QString WalletOperation::getExchange(void) const noexcept {return exchange_;};
QString WalletOperation::getCoin(void) const noexcept {return coinName_;}
double WalletOperation::getFiatPrice(void) const noexcept {return fiatPrice_;}
