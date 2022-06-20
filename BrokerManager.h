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
#include "IMarketData/Coingecko.h"

class BrokerManager : public QObject{
    Q_OBJECT

public:
    const uint8_t IMPORT_STAKING_OP_ATRS = 3;
    const uint8_t IMPORT_OP_ATRS = 9;

    bool FINISHED = false;
    inline static QString DEF_FIAT = "EUR";

    BrokerManager(const QObject* parent, OperationsModel*const operationsModel, WalletsModel*const walletsModel, WalletsModel*const walletsModelAll, WalletsPercModel*const walletsPercModel, CoinsModel*const coinsModel);
    inline static uint32_t userID ;
    inline static uint32_t year_;
    std::optional<std::vector<Wallet*>> findWallets(const QString& coin);
    std::optional<Wallet> findWallet(const QString& exchange, const QString& coin);
    Coin* findCoin(const QString& coin);
    static BrokerManager* getInstance(OperationsModel*const operationsModel, WalletsModel*const walletsModel, WalletsModel*const walletsModelAll, WalletsPercModel*const walletsPercModel, CoinsModel*const coinsModel) {
        if (!instance)
            instance = new BrokerManager(0, operationsModel, walletsModel, walletsModelAll, walletsPercModel, coinsModel);
        return instance;
    }


signals:
    void depositCompleted(void);



public slots:
    bool newDeposit(const int walletID, double pairAmount, double fees,
                    const QString comment, QString date);
    bool newOperation(const int walletID1,const int walletID2, double pair1Amount, double pair1AmountFiat,
                      double pair2Amount, double pair2AmountFiat, QString feesCoin, double comision, double comisionFiat, QString comments, QString type,
                      QString status, QString date);
    bool newOperation(const QString& exchange, const std::shared_ptr<Operation> operation);
    bool addWallet(const QString coinName, const QString exchange);
    bool importOperations(void);
    uint32_t getUserID(const QString& username);
    QStringList getWalletsCBox(const QString& username);
    void setUserID(const QString& username);
    void setYear(const QString& year);
    bool importPreviewOperations(const QString& csvFilePath, const QString& type);
    void updateCurrentPrice(void);
    std::optional<double>  getCurrentPrice(Coin* coin);

private:
    OperationsModel* operationsModel_;
    WalletsModel* walletsModel_;
    WalletsModel* walletsModelAll_;
    WalletsPercModel* walletsModelPerc_;
    CoinsModel* coinsModel_;
    std::vector<Operation*> importPreview;

    inline static BrokerManager *instance;


    void loadOperationsFromDB(const uint32_t userID);
    void loadOperationsFromDB(const uint32_t userID, const uint32_t year);
    void loadWalletsFromDB(const uint32_t userID);
    void loadCoinsFromDB(void);
    void loadDepositsFromDB(const uint32_t userID);
    void groupCoinBySymbol(void);
    void setCoinPtrInWallets();

};


#endif //BROKER_BROKERMANAGER_H
