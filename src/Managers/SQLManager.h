//
// Created by gabridc on 3/11/21.
//

#ifndef BROKER_SQLMANAGER_H
#define BROKER_SQLMANAGER_H
#include <QDir>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QTimeZone>
#include <Operation.h>
#include "Wallet.h"
#include "Config.h"
#include "WalletOperation.h"
#include "CoinsModel.h"
#include "WalletsModel.h"
#include "Deposit.h"
#include "Utils.h"

class SQLManager {

public:
    SQLManager(const SQLManager&) = delete;
    SQLManager(SQLManager&&) = delete;
    auto operator=(SQLManager &) -> SQLManager& = delete;
    auto operator=(SQLManager &&) -> SQLManager& = delete;


    static SQLManager *GetInstance(void);

    /*
     * Funciones comunes
     */
    bool init(void) const;
    bool update(void) const;
    uint32_t getVersion(void) const;
    bool openDatabase(void);
    uint32_t getUserID(const QString& username);
    QString getServer(void) const;
    QString getDatabase(void) const;
    static QSqlDatabase& getDb(void) { return database;};
    QList<std::tuple<uint32_t, QString, QString, QString>> getCoins(void);
    QList<std::tuple<uint32_t, QString, QString>> getExchanges(void);
    std::optional<std::tuple<uint32_t, QString, QString, QString>> getCoin(const QString& coin);
    QList<std::tuple<uint32_t, QString>> getAssetTypes(void);
    QList<std::tuple<uint32_t, QString>> getAssetTypes(const QString& category);
    QList<std::tuple<uint32_t, QString>> getUsers(void);
    std::optional<std::vector<Deposit*>> getDeposits(void);
    std::optional<std::vector<Deposit*>> getDeposits(const uint32_t user);
    std::tuple<bool, std::vector<Deposit*>> getDeposits(const QString& user, const QString& exchange) ;
    std::tuple<bool, std::vector<Wallet*>> getWallets(void);
    std::tuple<bool, std::vector<Wallet*>> getWallets(const uint32_t userID );
    std::tuple<bool, std::vector<Wallet*>> getWallets(const uint32_t userID, const QList<WalletsModel::AssetType> types);
    std::optional<Wallet> getWallets(const uint32_t userID, const QString& coin, const QString exchange);
    std::optional<std::vector<WalletOperation*>>  getWalletsOps(const uint32_t userID, const QString& coin, const QString exchange = "");
    std::optional<std::vector<Wallet*>> getNonCryptoWallets(const uint32_t userID);
    std::optional<std::vector<Wallet*>> getCryptoWallets(const uint32_t userID);
    std::tuple<bool, Wallet*> getWallet(const uint32_t user, const QString& exchange, const QString& coin);
    std::tuple<bool, Wallet*> getWallet(const uint32_t walletID);
    std::optional<std::vector<const WalletOperation*>> getWalletOperations(const QString& walletID);
    //virtual Wallet getWallet(const QString& wallet) = 0;
    //std::tuple<bool, std::vector<Operation*>> getOperations(void);
    std::tuple<bool, std::vector<Operation*>> getOperations(const uint32_t userID);
    std::tuple<bool, std::vector<Operation*>> getOperations(const QString& walletID);
    Operation* getLastOperation(void) const;
    std::vector<WalletOperation*> getLastNWalletOperation(int limit) const;
    QString getUsername(uint32_t userid);

    std::tuple<bool, std::vector<Operation*>> getOperations(const uint32_t userID, const QString& exchange);
    bool depositOperation(const int walletID, const QString exchange, double amount, double amountFiat, double fees, const QString& comments, QString& date);
    //virtual int getInvested(const QString& user, const QString& excahnge) = 0;
    double getInvested(const QString& user, const QString& exchange, const QString& wallet);
    int addWallet(const QString& coin, const QString& exchange, const uint32_t user);
    int getWalletID(const uint32_t user, const QString& exchange, const QString& coin);
    bool registerOperationNew(const std::vector<WalletOperation*> walletOperations,
                              const WalletOperation::OperationData& data,
                              std::vector<WalletOperation>& wOpsModified);
    bool registerAsset(const QString& type, const QString& name, const QString& color);
    bool registerUser(const QString& username);
    void setWalletData(Wallet& wallet);
    const QString   LinuxDatabasePath = QDir::homePath()+ "/.broker/broker (copia) (1).db";
    //const QString   LinuxDatabasePath = QDir::homePath()+ "/.broker/broker.db";

    // Auxiliary tables
    QDateTime DatetimeUTCStrToDatetime(QString timeUtc) const;
    QString updateDateTimeUTCFromQTFormat(QString table,QString id, QDateTime time, QString exchange);


    /*
     * Create database and tables functions
     */
    bool createTables(void);
    bool createAssetTypes(void) const;
    bool createVersion(void) const;
    bool createCoins(void) const;
    bool createUsers(void) const;
    bool createWallets(void) const;
    bool createWalletOperations(void) const;
    bool createOperations(void) const;
    bool createWithdraws(void) const;
    bool createDeposits(void) const;

    /*
     * DB updates
     */
    bool insertVersion(uint32_t version, QString remark) const;
    bool updateVersion(uint32_t version, QString remark) const;
    bool update200000(void) const;
    bool update201000(void) const;
    bool update201001(void) const;
    bool update203000(void) const;


    /*
     * Add default values
     */
    bool defaultCoins(void) const;
    bool defaultAssetTypes(void) const;


protected:
    static QSqlDatabase database;
    static QString server;
    static QString databaseName;

private:
    SQLManager() {};

    QString in(const QList<WalletsModel::AssetType> types) const;
    
    static SQLManager* instance_;
    std::tuple<bool, std::vector<Operation*>> processGetOperations(QSqlQuery& query);
    QString query_view_operations = "SELECT OP.date, OP.type, OP.wallet1, W1.exchange, C1.name, OP.pair1Amount, OP.pair1AmountFiat,"
                                    "    OP.wallet2, W2.exchange, C2.name, OP.pair2Amount, OP.pair2AmountFiat,"
                                    "    OP.comision, OP.comisionFiat, OP.ganancia"
                                    "    FROM Operations OP"
                                    "    LEFT JOIN Wallets W1 ON W1.id = OP.wallet1"
                                    "    LEFT JOIN Coins C1 ON C1.id = W1.coin"
                                    "    LEFT JOIN Users U1 ON U1.id = W1.user"
                                    "    LEFT JOIN Wallets W2 ON W2.id = OP.wallet1"
                                    "    LEFT JOIN Coins C2 ON C2.id = W2.coin"
                                    "    LEFT JOIN Users U2 ON U2.id = W2.user";

};


#endif //BROKER_SQLMANAGER_H
