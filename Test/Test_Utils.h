#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "SQLManager.h"

void deleteTablesData(QSqlQuery& query)
{
    query.prepare("DELETE FROM WalletOperations");
    query.exec();
    query.prepare("DELETE FROM Operations");
    query.exec();
    query.prepare("DELETE FROM Withdraws");
    query.exec();
    query.prepare("DELETE FROM Deposits");
    query.exec();
    query.prepare("DELETE FROM Wallets");
    query.exec();
    query.prepare("DELETE FROM Users");
    query.exec();    
}

#endif