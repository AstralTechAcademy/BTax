//
// Created by gabridc on 24/10/21.
//

#include "Operation.h"

int Operation::getID(void) const noexcept
{
    return id_;
}

double Operation::getBuyPrice(void) const noexcept
{
    return buyPrice_;
}

double Operation::getSellPrice(void) const noexcept
{
    return buyPrice_;
}

QString Operation::getPair1(void) const noexcept {
    return pair1_;
}

QString Operation::getPair2(void) const noexcept{
    return pair2_;
}

double Operation::getBuyPriceFiat(void) const noexcept{
    return buyPriceFiat_;
}

double Operation::getSellPriceFiat(void) const noexcept{
    return sellPriceFiat_;
}

double Operation::getDeposit(void) const noexcept{
    return deposit_;
}

double Operation::getRetired(void) const noexcept{
    return retired_;
}

QString Operation::getStatus(void) const noexcept{
    return status_;
}

QString Operation::getBuyDate(void) const noexcept{
    return buyDate_;
}

QString Operation::getSellDate(void) const noexcept{
    return sellDate_;
}

QString Operation::getComments(void) const noexcept{
    return comments_;
}


