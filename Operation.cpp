//
// Created by gabridc on 24/10/21.
//

#include "Operation.h"
#include <iostream>

int Operation::getID(void) const noexcept
{
    return id_;
}

QString Operation::getPair1(void) const noexcept {
    return pair1_;
}

QString Operation::getPair2(void) const noexcept{
    return pair2_;
}

QString Operation::getStatus(void) const noexcept{
    return status_;
}

QString Operation::getDate(void) const noexcept{
    return date_;
}

QString Operation::getComments(void) const noexcept{
    return comments_;
}

double Operation::getPair1Amount() const {
    return pair1Amount_;
}

double Operation::getPair1AmountFiat() const {
    return pair1AmountFiat_;
}

double Operation::getPair2Amount() const {
    return pair2Amount_;
}

double Operation::getPair2AmountFiat() const {
    return pair2AmountFiat_;
}

QString Operation::getFeesCoin() const {
    return feesCoin_;
}

double Operation::getComision() const {
    return comision_;
}

double Operation::getComisionFiat() const {
    return comisionFiat_;
}

QString Operation::getType() const {
    return type_;
}

double Operation::getGanancia() const {
    return ganancia_;
}

void Operation::setPair1Amount(double pair1Amount) {
    pair1Amount_ = pair1Amount;
}

void Operation::setPair1AmountFiat(double pair1AmountFiat) {
    pair1AmountFiat_ = pair1AmountFiat;
}

void Operation::setPair2Amount(double pair2Amount) {
    pair2Amount_ = pair2Amount;
}

void Operation::setPair2AmountFiat(double pair2AmountFiat) {
    pair2AmountFiat_ = pair2AmountFiat;
}

void Operation::setFeesCoin(QString feesCoin)
{
    feesCoin_ = feesCoin;
}

void Operation::setComision(double comision) {
    comision_ = comision;
}

void Operation::setComisionFiat(double comisionFiat) {
    comisionFiat_ = comisionFiat;
}

void Operation::setType(const QString &type) {
    type_ = type;
}

void Operation::setGanancia(double ganancia) {
    ganancia_ = ganancia;
}

void Operation::print(void) const
{
    std::cout << "Operation Data: " << id_ << pair1_.toStdString() << pair2_.toStdString() << " "  << pair1Amount_  << " " << pair1AmountFiat_ << " " << pair2Amount_ << " " << pair2AmountFiat_ << " " <<comision_ << " " <<comisionFiat_  << " " << status_.toStdString() << " " << date_.toStdString() << " " <<comments_.toStdString() << " " << type_.toStdString() << " " << ganancia_ << " "  << std::endl;

}
