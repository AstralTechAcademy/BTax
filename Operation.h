//
// Created by gabridc on 24/10/21.
//

#ifndef BROKER_OPERATION_H
#define BROKER_OPERATION_H
#include <QObject>

class Operation : public QObject{

    Q_PROPERTY(int id MEMBER id_ READ getID WRITE setID NOTIFY idChanged)
    Q_PROPERTY(QString pair1 MEMBER pair1_ READ getPair1 WRITE setPair1 NOTIFY pair1Changed)
    Q_PROPERTY(QString pair2 MEMBER pair2_ READ getPair2 WRITE setPair2 NOTIFY pair2Changed)
    Q_PROPERTY(double buyPrice MEMBER buyPrice_ READ getBuyPrice WRITE setBuyPrice NOTIFY buyPriceChanged)
    Q_PROPERTY(double sellPrice MEMBER sellPrice_ READ getSellPrice WRITE setSellPrice NOTIFY sellPriceChanged)
    Q_PROPERTY(double buyPriceFiat MEMBER buyPriceFiat_ READ getBuyPriceFiat WRITE setBuyPriceFiat NOTIFY buyPriceFiatChanged)
    Q_PROPERTY(double sellPriceFiat MEMBER sellPriceFiat_ READ getSellPriceFiat WRITE setSellPriceFiat NOTIFY sellPriceFiatChanged)
    Q_PROPERTY(double deposit MEMBER deposit_ READ getDeposit WRITE setDeposit NOTIFY depositChanged)
    Q_PROPERTY(double retired MEMBER retired_ READ getRetired WRITE setRetired NOTIFY retiredChanged)
    Q_PROPERTY(QString statusBuy MEMBER statusBuy_ READ getStatusBuy WRITE setStatusBuy NOTIFY statusBuyChanged)
    Q_PROPERTY(QString statusSell MEMBER statusSell_ READ getStatusSell WRITE setStatusSell NOTIFY statusSellChanged)
    Q_PROPERTY(QString buyDate MEMBER buyDate_ READ getBuyDate WRITE setBuyDate NOTIFY buyDateChanged)
    Q_PROPERTY(QString sellDate MEMBER sellDate_ READ getSellDate WRITE setSellDate NOTIFY sellDateChanged)
    Q_PROPERTY(QString comments MEMBER comments_ READ getComments WRITE setComments NOTIFY commentsChanged)
public:
    Operation(QObject *parent = 0);
    ~Operation(void) = default;
    Operation(int id, QString pair1, QString pair2, double buyPrice, double sellPrice,
              double buyPriceFiat, double sellPriceFiat, double deposit, double retired,
              QString statusBuy, QString statusSell, QString buyDate, QString sellDate, QString comments):
              id_(id),
              pair1_(pair1),
              pair2_(pair2),
              buyPrice_(buyPrice),
              sellPrice_(sellPrice),
              buyPriceFiat_(buyPriceFiat),
              sellPriceFiat_(sellPriceFiat),
              deposit_(deposit),
              retired_(retired),
              statusBuy_(statusBuy),
              statusSell_(statusSell),
              buyDate_(buyDate),
              sellDate_(sellDate),
              comments_(comments){};

    int getID(void) const noexcept;
    QString getPair1(void) const noexcept;
    QString getPair2(void) const noexcept;
    double getBuyPrice(void) const noexcept;
    double getSellPrice(void) const noexcept;
    double getBuyPriceFiat(void) const noexcept;
    double getSellPriceFiat(void) const noexcept;
    double getDeposit(void) const noexcept;
    double getRetired(void) const noexcept;
    QString getStatusBuy(void) const noexcept;
    QString getStatusSell(void) const noexcept;
    QString getBuyDate(void) const noexcept;
    QString getSellDate(void) const noexcept;
    QString getComments(void) const noexcept;

    void setID(const int id) noexcept;
    void setPair1(const QString pair1) noexcept;
    void setPair2(const QString pair2) noexcept;
    void setBuyPrice(const double value) noexcept;
    void setSellPrice(const double value) noexcept;
    void setBuyPriceFiat(const double value) noexcept;
    void setSellPriceFiat(const double value) noexcept;
    void setDeposit(const double value) noexcept;
    void setRetired(const double value) noexcept;
    void setStatusBuy(const QString status) noexcept;
    void setStatusSell(const QString status) noexcept;
    void setBuyDate(const QString buyDate) noexcept;
    void setSellDate(const QString sellDate) noexcept;
    void setComments(const QString ) noexcept;

private:
    int  id_;
    QString   pair1_;
    QString pair2_;
    double buyPrice_;
    double sellPrice_;
    double buyPriceFiat_;
    double sellPriceFiat_;
    double deposit_;
    double retired_;
    QString statusBuy_;
    QString statusSell_;
    QString buyDate_;
    QString sellDate_;
    QString comments_;

};


#endif //BROKER_OPERATION_H
