//
// Created by gabridc on 9/11/21.
//

#include "Wallet.h"
#include <iostream>

void Wallet::print() const noexcept
{
    //std::cout << "History Total Ammount: " << amount << " " << coin.toStdString() << std::endl;
    std::cout << "Current Invested: " << invested << " " << "   EUR" << std::endl;
    std::cout << "Available: " << available << " " << coin.toStdString() << std::endl;
    std::cout << "Retired: " << retired << " " << coin.toStdString() << std::endl;
    std::cout << "Current Average Price: " << invested /  available << std::endl;
}
