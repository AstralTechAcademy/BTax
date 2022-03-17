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
    Deposit(const QString &coin, double amount, double comision, const QDate &date,
                     const QString &exchange, const QString &user) :
                     coin(coin), amount(amount),
                     comision(comision),
                     date(date),
                     exchange(exchange),
                     user(user) {}

    void print(void) const noexcept;
    const QString& getCoin() const ;
    double getAmount() const ;

    double getComision() const ;

    double getAvailable() const ;

    const QDate &getDate() const ;
    const QString &getExchange() const ;

    const QString &getUser() const;


private:
    QString coin;
    double amount;
    double comision;
    double available;
    QDate date;
    QString exchange;
    QString user;

};


#endif //BROKER_DEPOSIT_H
