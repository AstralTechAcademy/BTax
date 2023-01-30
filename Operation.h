//
// Created by gabridc on 24/10/21.
//

#ifndef BROKER_OPERATION_H
#define BROKER_OPERATION_H
#include <QObject>
#include <QDateTime>
#include <iostream>
#include "Utils.h"
class Operation : public QObject{

    Q_PROPERTY(int id MEMBER id_ READ getID WRITE setID NOTIFY idChanged)
    Q_PROPERTY(QString pair1 MEMBER pair1_ READ getPair1 WRITE setPair1 NOTIFY pair1Changed)
    Q_PROPERTY(QString walletID1 MEMBER walletID1_ READ getWalletID1 WRITE setWalletID1 NOTIFY WalletID1Changed)
    Q_PROPERTY(QString pair2 MEMBER pair2_ READ getPair2 WRITE setPair2 NOTIFY pair2Changed)
    Q_PROPERTY(QString walletID2 MEMBER walletID2_ READ getWalletID2 WRITE setWalletID2 NOTIFY WalletID2Changed)
    Q_PROPERTY(double pair1Amount MEMBER pair1Amount_ READ getPair1Amount WRITE setPair1Amount NOTIFY pair1AmountChanged)
    Q_PROPERTY(double pair1AmountFiat MEMBER pair1AmountFiat_ READ getPair1AmountFiat WRITE setPair1AmountFiat NOTIFY pair1AmountFiatChanged)
    Q_PROPERTY(double pair2Amount MEMBER pair2Amount_ READ getPair2Amount WRITE setPair2Amount NOTIFY pair2AmountChanged)
    Q_PROPERTY(double pair2AmountFiat MEMBER pair2AmountFiat_ READ getPair2AmountFiat WRITE setPair2AmountFiat NOTIFY pair2AmountFiatChanged)
    Q_PROPERTY(double comision MEMBER comision_ READ getComision WRITE setComision NOTIFY comisionChanged)
    Q_PROPERTY(double comisionFiat MEMBER comisionFiat_ READ getComisionFiat WRITE setComisionFiat NOTIFY comisionFiatChanged)
    Q_PROPERTY(QString status MEMBER status_ READ getStatus WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString date MEMBER date_ READ getDate WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QString comments MEMBER comments_ READ getComments WRITE setComments NOTIFY commentsChanged)
    Q_PROPERTY(QString type MEMBER type_ READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(double ganancia MEMBER ganancia_ READ getGanancia WRITE setGanancia NOTIFY gananciaChanged)
public:
    Operation(QObject *parent = 0);
    ~Operation(void) = default;
    Operation(int id, QString pair1, QString pair2, double pair1Amount, double pair1AmountFiat,
              double pair2Amount, double pair2AmountFiat, double comision, double comisionFiat,
              QString status, QString date, QString comments, QString type, double ganancia):
              id_(id),
              pair1_(pair1),
              pair2_(pair2),
              pair1Amount_(pair1Amount),
              pair1AmountFiat_(pair1AmountFiat),
              pair2Amount_(pair2Amount),
              pair2AmountFiat_(pair2AmountFiat),
              comision_(comision),
              comisionFiat_(comisionFiat),
              status_(status),
              date_(date),
              comments_(comments),
              type_(type),
              ganancia_(ganancia){};
    Operation(int id, QString pair1, QString pair2, double pair1Amount, double pair1AmountFiat,
              double pair2Amount, double pair2AmountFiat, QString feesCoin, double comision, double comisionFiat,
              QString status, QString date, QString comments, QString type, double ganancia):
            id_(id),
            pair1_(pair1),
            pair2_(pair2),
            pair1Amount_(pair1Amount),
            pair1AmountFiat_(pair1AmountFiat),
            pair2Amount_(pair2Amount),
            pair2AmountFiat_(pair2AmountFiat),
            feesCoin_(feesCoin),
            comision_(comision),
            comisionFiat_(comisionFiat),
            status_(status),
            date_(date),
            comments_(comments),
            type_(type),
            ganancia_(ganancia){};

    Operation(int id, QString pair1, QString pair2, int walletID1, int walletID2, double pair1Amount, double pair1AmountFiat,
              double pair2Amount, double pair2AmountFiat, QString feesCoin, double comision, double comisionFiat,
              QString status, QString date, QString comments, QString type, double ganancia):
            id_(id),
            pair1_(pair1),
            pair2_(pair2),
            walletID1_(walletID1),
            walletID2_(walletID2),
            pair1Amount_(pair1Amount),
            pair1AmountFiat_(pair1AmountFiat),
            pair2Amount_(pair2Amount),
            pair2AmountFiat_(pair2AmountFiat),
            feesCoin_(feesCoin),
            comision_(comision),
            comisionFiat_(comisionFiat),
            status_(status),
            date_(date),
            comments_(comments),
            type_(type),
            ganancia_(ganancia){};

    int getID(void) const noexcept;
    QString getPair1(void) const noexcept;
    QString getPair2(void) const noexcept;
    int getWalletID1(void) const noexcept;
    int getWalletID2(void) const noexcept;
    QString getStatus(void) const noexcept;
    QString getDate(void) const noexcept;
    QDateTime getDateTime(void) const noexcept;
    QString getComments(void) const noexcept;
    double getPair1Amount() const;
    double getPair1AmountFiat() const;
    double getPair2Amount() const;
    double getPair2AmountFiat() const;
    QString getFeesCoin() const;
    double getComision() const;
    double getComisionFiat() const;
    QString getType() const;
    double getGanancia() const;

    void setID(const int id) noexcept;
    void setPair1(const QString pair1) noexcept;
    void setPair2(const QString pair2) noexcept;
    void setWalletID1(const int walletID1) noexcept;
    void setWalletID2(const int walletID1) noexcept;
    void setStatus(const QString status) noexcept;
    void setDate(const QString buyDate) noexcept;
    void setComments(const QString ) noexcept;
    void setPair1Amount(double pair1Amount);
    void setPair1AmountFiat(double pair1AmountFiat);
    void setPair2Amount(double pair2Amount);
    void setPair2AmountFiat(double pair2AmountFiat);
    void setFeesCoin(QString feesCoin);
    void setComision(double comision);
    void setComisionFiat(double comisionFiat);
    void setType(const QString &type);
    void setGanancia(double ganancia);
    void print(void) const;

    bool operator==(Operation& rhs)
    {
        auto date = rhs.getDate();
        QDateTime datetime;
        cnvStrToDateTime(date, datetime);
        qDebug() << date << " " << BTime::toString(datetime) << " " << 
                    BTime::toString(QLocale(QLocale::Spanish).toDateTime(date_, "ddd. MMM. d hh:mm:ss yyyy")) ;
        return date_ == date &&
                pair1_ == rhs.getPair1() && pair1Amount_ == rhs.getPair1Amount() &&
                pair2_ == rhs.getPair2() && pair2Amount_== rhs.getPair2Amount() &&
                type_ == rhs.getType();
    }

    enum class EN_OperationColumns_t {
        ID = 0,
        WALLET1,
        WALLET2,
        PAIRA1AMOUNT,
        PAIRA1AMOUNTFIAT,
        PAIR2AMOUNT,
        PAIR2AMOUNTFIAT,
        COMISION,
        COMISIONFIAT,
        GANANCIA,
        STATUS,
        DATE,
        COMMENTS,
        TYPE
    };

private:
    int  id_;
    QString pair1_;
    QString pair2_;
    int walletID1_;
    int walletID2_;
    double pair1Amount_;
    double pair1AmountFiat_;
    double pair2Amount_;
    double pair2AmountFiat_;
    double comision_;
    double comisionFiat_;
    QString status_;
    QString date_;
    QString dateDBFormat_;
    QString comments_;
    QString type_;
    double ganancia_;
    QString feesCoin_;

};


#endif //BROKER_OPERATION_H
