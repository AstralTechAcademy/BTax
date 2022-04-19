//
// Created by gabridc on 26/11/21.
//

#ifndef BROKER_BROKERMANAGER_H
#define BROKER_BROKERMANAGER_H
#include <QObject>
#include "OperationsModel.h"
#include "WalletsModel.h"
#include "WalletsPercModel.h"
#include "CoinsModel.h"
#include "DBLocal.h"
#include "DBRemote.h"

class BrokerManager : public QObject{
    Q_OBJECT

public:
    const uint8_t IMPORT_STAKING_OP_ATRS = 3;
    const uint8_t IMPORT_OP_ATRS = 9;

    BrokerManager(const QObject* parent, OperationsModel*const operationsModel, WalletsModel*const walletsModel,WalletsModel*const walletsModelDeposit, WalletsPercModel*const walletsPercModel, CoinsModel*const coinsModel);
    static uint32_t userID ;

signals:
    void depositCompleted(void);



public slots:
    bool newDeposit(const int walletID, double pairAmount, double fees,
                    const QString comment, QString date);
    bool newOperation(const int walletID1,const int walletID2, double pair1Amount, double pair1AmountFiat,
                      double pair2Amount, double pair2AmountFiat, QString feesCoin, double comision, double comisionFiat, QString comments, QString type,
                      QString status, QString date);
    bool addWallet(const QString coinName, const QString exchange);
    bool importOperations(void);
    uint32_t getUserID(const QString& username);
    QStringList getWalletsCBox(const QString& username);
    void setUserID(const QString& username);
    void setYear(const QString& year);
    bool importPreviewOperations(const QString& csvFilePath, const QString& type);

private:
    OperationsModel* operationsModel_;
    WalletsModel* walletsModel_;
    WalletsModel* walletsModelDeposit_;
    WalletsPercModel* walletsModelPerc_;
    CoinsModel* coinsModel_;
    std::vector<Operation*> importPreview;


    void loadOperationsFromDB(const uint32_t userID);
    void loadWalletsFromDB(const uint32_t userID);
    void loadCoinsFromDB(void);
    void loadDepositsFromDB(const uint32_t userID);
    void groupCoinBySymbol(void);

};


#endif //BROKER_BROKERMANAGER_H
