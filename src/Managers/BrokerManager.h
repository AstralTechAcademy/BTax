//
// Created by gabridc on 26/11/21.
//

#ifndef BROKER_BROKERMANAGER_H
#define BROKER_BROKERMANAGER_H
#include <QObject>
#include "OperationsModel.h"
#include "WalletsModel.h"
#include "WalletsPercModel.h"
#include "WalletsFilterBarManager.h"
#include "CoinsModel.h"
#include "ExchangesModel.h"
#include "AssetTypeModel.h"
#include "SQLManager.h"
#include "IMarketData/Coingecko.h"
#include "logger.h"

class BrokerManager : public QObject{
    Q_OBJECT
    friend class Importer;
public:

    enum class NewOperationRes
    {
        NOT_ADDED = 0,
        ADDED,
        ALREADY_ADDED,
        ORI_WALLET_NOT_EXIST,
        DEST_WALLET_NOT_EXIST,
        INSUF_BALANCE_ORI_WALLET,
        VALIDATION_ERROR,
        COIN_NOT_FOUND,
        ORI_COIN_NOT_EXIST,
        DEST_COIN_NOT_EXIST,
        OK

    };

    enum class LoadResCode
    {
        UNKNOWN = 0,
        OK = 1,
        NO_USERS
    };

    struct NewOperationData
    {
        const int walletID1;
        const int walletID2;
        double pair1Amount;
        double pair1AmountFiat;
        double pair2Amount;
        double pair2AmountFiat;
        QString feesCoin;
        double comision;
        double comisionFiat;
        QString comments;
        QString type;
        QString status;
        QString date;
    };

    const uint8_t IMPORT_STAKING_OP_ATRS = 3;
    const uint8_t IMPORT_OP_ATRS = 9;

    bool FINISHED = false;
    inline static QString DEF_FIAT = "EUR";
    inline static QString DEF_FIAT_ID = "7";

    BrokerManager(const QObject* parent, OperationsModel*const operationsModel, WalletsModel*const walletsModel,WalletsModel*const walletsModelAll,
                    WalletsPercModel*const walletsPercModel, CoinsModel*const coinsModel, AssetTypeModel*const assetTypesModel, 
                    ExchangesModel*const exchangesModel);
    inline static uint32_t userID ;
    inline static uint32_t year_;
    std::optional<std::vector<Wallet*>> findWallets(const QString& coin);
    std::optional<Wallet> findWallet(const QString& exchange, const QString& coin);
    std::optional<Coin*> findCoin(const QString& coin);
    static BrokerManager* getInstance(OperationsModel*const operationsModel, WalletsModel*const walletsModel, WalletsModel*const walletsModelAll,
                                      WalletsPercModel*const walletsPercModel, CoinsModel*const coinsModel, AssetTypeModel*const assetTypesModel,
                                      ExchangesModel*const exchangesModel) {
        if (!instance)
            instance = new BrokerManager(0, operationsModel, walletsModel, walletsModelAll, walletsPercModel, coinsModel, assetTypesModel, exchangesModel);
        return instance;
    }

    static BrokerManager* getInstance(void) {
        return instance;
    }
    bool checkDuplicity(const QString& exchange, std::shared_ptr<Operation> operation);
    std::optional<std::vector<WalletOperation*>>  getAvailableBalancesOrdered(const QString& coinID, const QString exchange = "");

    LoadResCode load(void);
    Operation* getLastOperation(void) const;
    std::vector<WalletOperation*> getLastNWalletOperation(int limit = 1) const;
    int newOperation(const QString& exchange, const std::shared_ptr<Operation> operation, std::vector<WalletOperation>& wOpsModified);
    int setWallets(const QString& exchange, std::shared_ptr<Operation> operation);
signals:
    void depositCompleted(void);



public slots:
    bool newUser(const QString& name);
    bool newDeposit(const int walletID, double pairAmount, double fees,
                    const QString comment, QString date);
    int newOperation(const int walletID1,const int walletID2, double pair1Amount, double pair1AmountFiat,
                     double pair2Amount, double pair2AmountFiat, QString feesCoin, double comision, double comisionFiat, QString comments, QString type,
                     QString status, QString date);
    int newTransfer(const int walletID1,const int walletID2, double walletOAmount, double walletDAmount, QString feesCoin, 
                        double comision, double comisionFiat, QString comments, QString status, QString date);
    bool newAssetIfNotExist(const QString& type, const QString& name, const QString& color);
    bool newAsset(const QString& type, const QString& name, const QString& color);
    bool addWalletIfNotExist(const QString coinName, const QString exchange);
    bool addWallet(const QString coinName, const QString exchange);
    uint32_t getUserID(const QString& username);
    QStringList getWalletsCBox(const QString& username);
    void setUserID(const QString& username);
    void setYear(const QString& year);
    void updateCurrentPrice(void);
    std::optional<double> getCurrentPrice(Coin* coin);

    // From filter bars C++ classes
    void updateAssetTypeModel(const QString& category);
    void filterWallets();



private:
    inline static BrokerManager *instance;

    void filterAssetTypes(const QString& category);

#ifdef GTEST
public:
#endif
    OperationsModel* operationsModel_;
    WalletsModel* walletsModel_;
    WalletsModel* walletsModelAll_;
    WalletsPercModel* walletsModelPerc_;
    CoinsModel* coinsModel_;
    ExchangesModel* exchangesModel_;
    AssetTypeModel* assetTypesModel_;
    std::vector<Operation*> operationsToAdd;

    void loadOperationsFromDB(const uint32_t userID);
    void loadOperationsFromDB(const uint32_t userID, const uint32_t year);
    void loadWalletsFromDB(const uint32_t userID, const QList<WalletsModel::AssetType> assetTypes = {WalletsModel::AssetType::ALL});
    void loadCoinsFromDB(void);
    void loadExchangesFromDB(void);
    void loadAssetTypesFromDB(void);
    void loadDepositsFromDB(const uint32_t userID);
    void groupCoinBySymbol(void);
    void setCoinPtrInWallets();
    double getAvailableAmounts(const std::vector<WalletOperation*>& wallets) const;


    int newOperation(WalletOperation::OperationData data, std::vector<WalletOperation>& wOpsModified);
    int newTransfer(WalletOperation::OperationData data, std::vector<WalletOperation>& wOpsModified);


};


#endif //BROKER_BROKERMANAGER_H
