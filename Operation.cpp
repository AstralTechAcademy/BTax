//
// Created by gabridc on 24/10/21.
//

#include "Operation.h"
#include <iostream>

int Operation::getID(void) const noexcept
{
    return id_;
}

double Operation::getPrice(void) const noexcept
{
    return price_;
}

QString Operation::getPair1(void) const noexcept {
    return pair1_;
}

QString Operation::getPair2(void) const noexcept{
    return pair2_;
}

double Operation::getPriceFiat(void) const noexcept{
    return priceFiat_;
}


double Operation::getDeposit(void) const noexcept{
    return deposit_;
}


QString Operation::getStatus(void) const noexcept{
    return status_;
}

QString Operation::getDate(void) const noexcept{
    std::cout << "GET DATE" << std::endl;
    return date_;
}

QString Operation::getComments(void) const noexcept{
    return comments_;
}

void Operation::setDeposit(QString value) noexcept{
    std::cout << "Set deposit" <<std::endl;
    deposit_ = value.toDouble();

}

void Operation::setDepositSlot(QString value) noexcept{
    deposit_ = value.toDouble();
    std::cout << "Set deposit" <<std::endl;
}