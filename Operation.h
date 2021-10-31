//
// Created by gabridc on 24/10/21.
//

#ifndef BROKER_OPERATION_H
#define BROKER_OPERATION_H
#include <QObject>
#include <iostream>

class Operation : public QObject{

    Q_PROPERTY(int id MEMBER id_ READ getID WRITE setID NOTIFY idChanged)
    Q_PROPERTY(QString pair1 MEMBER pair1_ READ getPair1 WRITE setPair1 NOTIFY pair1Changed)
    Q_PROPERTY(QString pair2 MEMBER pair2_ READ getPair2 WRITE setPair2 NOTIFY pair2Changed)
    Q_PROPERTY(double price MEMBER price_ READ getPrice WRITE setPrice NOTIFY priceChanged)
    Q_PROPERTY(double priceFiat MEMBER priceFiat_ READ getPriceFiat WRITE setPriceFiat NOTIFY priceFiatChanged)
    Q_PROPERTY(double deposit MEMBER deposit_ READ getDeposit WRITE setDeposit NOTIFY depositChanged)
    Q_PROPERTY(double retired MEMBER retired_ READ getRetired WRITE setRetired NOTIFY retiredChanged)
    Q_PROPERTY(QString status MEMBER status_ READ getStatus WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString date MEMBER date_ READ getDate WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QString comments MEMBER comments_ READ getComments WRITE setComments NOTIFY commentsChanged)
public:
    Operation(QObject *parent = 0);
    ~Operation(void) = default;
    Operation(int id, QString pair1, QString pair2, double price,
              double priceFiat, double deposit,
              QString status, QString date, QString comments):
              id_(id),
              pair1_(pair1),
              pair2_(pair2),
              price_(price),
              priceFiat_(priceFiat),
              deposit_(deposit),
              status_(status),
              date_(date),
              comments_(comments){};

    int getID(void) const noexcept;
    QString getPair1(void) const noexcept;
    QString getPair2(void) const noexcept;
    double getPrice(void) const noexcept;
    double getPriceFiat(void) const noexcept;
    double getDeposit(void) const noexcept;
    QString getStatus(void) const noexcept;
    QString getDate(void) const noexcept;
    QString getComments(void) const noexcept;

    void setID(const int id) noexcept;
    void setPair1(const QString pair1) noexcept;
    void setPair2(const QString pair2) noexcept;
    void setPrice(const double value) noexcept;
    void setPriceFiat(const double value) noexcept;
    void setDeposit(const QString value) noexcept;
    void setStatus(const QString status) noexcept;
    void setDate(const QString buyDate) noexcept;
    void setComments(const QString ) noexcept;

public slots:
    void setDepositSlot(const QString value) noexcept;
private:
    int  id_;
    QString   pair1_;
    QString pair2_;
    double price_;
    double priceFiat_;
    double deposit_;
    QString status_;
    QString date_;
    QString comments_;

};


#endif //BROKER_OPERATION_H
