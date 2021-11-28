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
    virtual std::tuple<bool, std::vector<Deposit*>> getDeposits(void) = 0;
    virtual std::tuple<bool, std::vector<Deposit*>> getDeposits(const QString& user) = 0;
    virtual std::tuple<bool, std::vector<Deposit*>> getDeposits(const QString& user, const QString& exchange) = 0;
    virtual std::tuple<bool, std::vector<Wallet*>> getWallets(const QString& user ) = 0;
    //virtual Wallet getWallet(const QString& wallet) = 0;
    virtual std::tuple<bool, std::vector<Operation*>> getOperations(void) = 0;
    virtual std::tuple<bool, std::vector<Operation*>> getOperations(const QString& exchange) = 0;
    //virtual int getInvested(const QString& user, const QString& excahnge) = 0;
    virtual double getInvested(const QString& user, const QString& exchange, const QString& wallet) = 0;
    virtual int addWallet(const QString& coin, double amount, const QString& exchange, const QString& user) = 0;
    virtual int getWalletID(const QString& user, const QString& exchange, const QString& coin) = 0 ;

    const QString   LinuxDatabasePath = QDir::homePath()+ "/.broker/broker (copia) (1).db";


};


#endif //BROKER_SQLMANAGER_H
