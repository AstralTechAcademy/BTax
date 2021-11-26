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

private:
    OperationsModel* operationsModel_;
    WalletsModel* walletsModel_;

};


#endif //BROKER_BROKERMANAGER_H
