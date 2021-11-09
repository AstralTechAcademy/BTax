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
    virtual void getWallets(void) = 0;
    virtual Wallet getWallet(const QString& wallet) = 0;
    virtual std::tuple<bool, std::vector<Operation*>> getOperations(void) = 0;

    const QString LinuxDatabasePath = QDir::homePath()+ "/.broker/broker (copia) (1).db";


};


#endif //BROKER_SQLMANAGER_H
