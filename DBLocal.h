//
// Created by gabridc on 7/11/21.
//

#ifndef BROKER_DBLOCAL_H
#define BROKER_DBLOCAL_H
#include "SQLManager.h"
#include "Wallet.h"
#include "Deposit.h"
#include <iostream>

class DBLocal : public SQLManager {

public:
    DBLocal() : SQLManager(){};

    DBLocal(const DBLocal&) = delete;
    DBLocal(DBLocal&&) = delete;
    auto operator=(DBLocal &) -> DBLocal& = delete;
    auto operator=(DBLocal &&) -> DBLocal& = delete;

    static DBLocal *GetInstance(void);

    /*
     * Funciones comunes
     */
    bool createDatabase(void) override;

    bool openDatabase(void) override;




private:
    static DBLocal* db_;
    QString databasePath;
};





#endif //BROKER_DBLOCAL_H
