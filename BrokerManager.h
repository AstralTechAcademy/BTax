//
// Created by gabridc on 26/11/21.
//

#ifndef BROKER_BROKERMANAGER_H
#define BROKER_BROKERMANAGER_H
#include <QObject>
#include "OperationsModel.h"
#include "WalletsModel.h"
#include "DBLocal.h"

class BrokerManager : public QObject{
    Q_OBJECT

public:
    BrokerManager(const QObject* parent, OperationsModel*const operationsModel, WalletsModel*const walletsModel);

public slots:
    bool newDeposit(const QString user, const QString exchange, const QString pair, double pairAmount, double fees,
                    const QString comment, QString date);
    bool newOperation(const QString user, const QString exchange, QString pair1, QString pair2, double pair1Amount, double pair1AmountFiat,
                      double pair2Amount, double pair2AmountFiat, double comision, double comisionFiat, QString comments, QString type,
                      QString status, QString date);
private:
    OperationsModel* operationsModel_;
    WalletsModel* walletsModel_;

    void loadOperationsFromDB(void);
    void loadWalletsFromDB(const QString& user);

};


#endif //BROKER_BROKERMANAGER_H
