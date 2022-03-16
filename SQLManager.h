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
#include <Operation.h>
#include "Wallet.h"
#include "Deposit.h"

class SQLManager {

public:
    SQLManager() {};

    SQLManager(const SQLManager&) = delete;
    SQLManager(SQLManager&&) = delete;
    auto operator=(SQLManager &) -> SQLManager& = delete;
    auto operator=(SQLManager &&) -> SQLManager& = delete;


    /*
     * Funciones comunes
     */
    virtual bool createDatabase(void) = 0;
    virtual bool openDatabase(void) = 0;
    uint32_t getUserID(const QString& username);
    QString getServer(void) const;
    QList<std::tuple<uint32_t, QString>> getCoins(void);
    QList<std::tuple<uint32_t, QString>> getUsers(void);
    std::tuple<bool, std::vector<Deposit*>> getDeposits(void);
    std::tuple<bool, std::vector<Deposit*>> getDeposits(const QString& user);
    std::tuple<bool, std::vector<Deposit*>> getDeposits(const QString& user, const QString& exchange) ;
    std::tuple<bool, std::vector<Wallet*>> getWallets(void);
    std::tuple<bool, std::vector<Wallet*>> getWallets(const uint32_t userID );
    std::tuple<bool, Wallet*> getWallet(const uint32_t user, const QString& exchange, const QString& coin);
    std::tuple<bool, Wallet*> getWallet(const uint32_t walletID);
    //virtual Wallet getWallet(const QString& wallet) = 0;
    //std::tuple<bool, std::vector<Operation*>> getOperations(void);
    std::tuple<bool, std::vector<Operation*>> getOperations(const uint32_t userID);
    std::tuple<bool, std::vector<Operation*>> getOperations(const uint32_t userID, const QString& exchange);
    bool depositOperation(const int walletID, double amount, double amountFiat, double fees, const QString& comments, QString& date);
    //virtual int getInvested(const QString& user, const QString& excahnge) = 0;
    double getInvested(const QString& user, const QString& exchange, const QString& wallet);
    int addWallet(const QString& coin, double amount, const QString& exchange, const uint32_t user);
    int getWalletID(const uint32_t user, const QString& exchange, const QString& coin);
    bool registerOperation(const int walletID1, const int walletID2, double pair1Amount, double pair1AmountFiat,
                           double pair2Amount, double pair2AmountFiat, QString feesCoin, double comision, double comisionFiat, QString& comments, QString& type,
                           QString& status, QString& date );
    void setWalletData(Wallet& wallet);
    const QString   LinuxDatabasePath = QDir::homePath()+ "/.broker/broker (copia) (1).db";
    //const QString   LinuxDatabasePath = QDir::homePath()+ "/.broker/broker.db";



protected:
    static QSqlDatabase database;
    static QString server;

};


#endif //BROKER_SQLMANAGER_H
