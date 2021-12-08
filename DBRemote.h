//
// Created by gabridc on 6/12/21.
//

#ifndef BROKER_DBREMOTE_H
#define BROKER_DBREMOTE_H

#include "SQLManager.h"
#include "Wallet.h"
#include "Deposit.h"
#include <iostream>

class DBRemote : public SQLManager {

public:
    DBRemote() : SQLManager(){};

    DBRemote(const DBRemote&) = delete;
    DBRemote(DBRemote&&) = delete;
    auto operator=(DBRemote &) -> DBRemote& = delete;
    auto operator=(DBRemote &&) -> DBRemote& = delete;

    static DBRemote *GetInstance(void);

    /*
     * Funciones comunes
     */
    bool createDatabase(void) override;

    bool openDatabase(void) override;

private:
    static DBRemote* db_;
    QString databasePath;
};


#endif //BROKER_DBREMOTE_H
