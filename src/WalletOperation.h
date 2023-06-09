//
// Created by gabridc on 9/11/21.
//

#ifndef BROKER_WALLET_OPERATION_H
#define BROKER_WALLET_OPERATION_H
#include <QString>
#include <iostream>
#include "CoinsModel.h"
#include "Operation.h"
#include <memory>
#include <QDateTime>

class WalletOperation {

public:
    struct OperationData{
        int walletID1;
        int walletID2;
        double pair1Amount;
        double pair1AmountFiat;
        double pair2Amount;
        double pair2AmountFiat;
        QString feesCoin;
        double comision;
        double comisionFiat;
        QString comments;
        QString type;
        QString status;
        QString date;
        QString datetimeUTC;

        void fromOperation(std::shared_ptr<Operation> o) noexcept
        {
            walletID1 = o->getWalletID1();
            walletID2 = o->getWalletID2();
            pair1Amount = o->getPair1Amount();
            pair1AmountFiat = o->getPair1AmountFiat();
            pair2Amount = o->getPair2Amount();
            pair2AmountFiat = o->getPair2AmountFiat();
            feesCoin = o->getFeesCoin();
            comision = o->getComision();
            comisionFiat = o->getComisionFiat();
            comments = o->getComments();
            type = o->getType();
            status = o->getStatus();
            date = o->getDate();
        }

    };

    WalletOperation() = delete;
    ~WalletOperation() = default;
    WalletOperation( int id, int walletID, const QString &coinName, const QString& exchange, const QString& user,
                     const double amount, const double available, const double retired, const double fiat, const QDateTime date, const QDateTime datetimeUtc) :
            id_(id),
            walletID_(walletID),
            coinName_(coinName),
            exchange_(exchange),
            user_(user),
            amount_(amount),
            retired_(retired),
            available_(available),
            fiatPrice_(fiat),
            date_(date),
            datetimeUTC_(datetimeUtc){};

    [[deprecated("Do not use at least you want to get LastWalletOperations")]]
    WalletOperation( int id, int walletID, const double amount, const double available, const double retired, const double fiat, const QDateTime date) :
            id_(id),
            walletID_(walletID),
            amount_(amount),
            retired_(retired),
            available_(available),
            fiatPrice_(fiat),
            date_(date){};

    void print(void) const noexcept;
    void print2(void) const noexcept;
    int getWalletID(void) const noexcept;
    int getID(void) const noexcept;
    QDateTime getDate(void) const noexcept;
    QDateTime getDateTimeUtc(void) const noexcept;
    double getAmount(void) const noexcept;
    double getRetired(void) const noexcept;
    double getAvailable(void) const noexcept;
    QString getUser(void) const noexcept;
    QString getExchange(void) const noexcept;
    QString getCoin(void) const noexcept;
    QString getFiatCoin(void) const noexcept;
    double getFiatPrice(void) const noexcept;

private:
    double fiatPrice_;
    double amount_;
    QString coinName_;
    QString exchange_;
    QString fiatCoin_;
    QString user_;
    int walletID_;
    double retired_;
    Coin* coin_;
    double available_;
    QDateTime date_;
    QDateTime datetimeUTC_ = QDateTime();
    int id_;
};


#endif //BROKER_WALLET_H
