//
// Created by gabridc on 26/11/21.
//

#ifndef BROKER_BROKERMANAGER_H
#define BROKER_BROKERMANAGER_H
#include <QObject>
#include "OperationsModel.h"
#include "WalletsModel.h"
#include "DBLocal.h"
#include "DBRemote.h"

class BrokerManager : public QObject{
    Q_OBJECT

public:
    const uint8_t IMPORT_STAKING_OP_ATRS = 3;
    const uint8_t IMPORT_OP_ATRS = 9;

    BrokerManager(const QObject* parent, OperationsModel*const operationsModel, WalletsModel*const walletsModel);
    static uint32_t userID ;

public slots:
    bool newDeposit(const int user, const QString exchange, const QString pair, double pairAmount, double fees,
                    const QString comment, QString date);
    bool newOperation(const int user, const QString exchange, QString pair1, QString pair2, double pair1Amount, double pair1AmountFiat,
                      double pair2Amount, double pair2AmountFiat, double comision, double comisionFiat, QString comments, QString type,
                      QString status, QString date);
    bool importOperations(void);
    uint32_t getUserID(const QString& username);
    void setUserID(const QString& username);
    bool importPreviewOperations(const QString& csvFilePath, const QString& type);

private:
    OperationsModel* operationsModel_;
    WalletsModel* walletsModel_;
    std::vector<Operation*> importPreview;


    void loadOperationsFromDB(void);
    void loadWalletsFromDB(const uint32_t userID);

};


#endif //BROKER_BROKERMANAGER_H
