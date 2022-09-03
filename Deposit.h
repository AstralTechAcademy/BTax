//
// Created by gabridc on 10/11/21.
//

#ifndef BROKER_DEPOSIT_H
#define BROKER_DEPOSIT_H
#include <QString>
#include <QDate>

class Deposit {

public:
    Deposit() = delete;
    ~Deposit() = default;
    Deposit(const QString &coin, double amount, double comision, const QDateTime &date, const QString &walletID) :
                     coin(coin), amount(amount),
                     comision(comision),
                     date(date),
                     walletID(walletID){}

    void print(void) const noexcept;
    const QString& getCoin() const ;
    double getAmount() const ;

    double getComision() const ;

    double getAvailable() const ;

    const QDateTime &getDate() const ;
    const QString &getWalletID() const ;

    const QString &getUser() const;


private:
    QString coin;
    double amount;
    double comision;
    double available;
    QDateTime date;
    QString walletID;
    QString user;

};


#endif //BROKER_DEPOSIT_H
