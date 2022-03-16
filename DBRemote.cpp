//
// Created by gabridc on 6/12/21.
//

#include "DBRemote.h"

DBRemote* DBRemote::db_ = nullptr;

/**
 * Static methods should be defined outside the class.
 */
DBRemote *DBRemote::GetInstance(void)
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if(db_ == nullptr){
        db_ = new DBRemote();
    }
    return db_;
}

bool DBRemote::createDatabase(void)
{
    bool created = false;
    //TODO

    return created;
};

bool DBRemote::openDatabase(void)
{
    bool opened = false;
    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setUserName("zendaya");
#ifdef OFICIAL
    database.setPort(53306);
    database.setHostName("zendaya.astraltech.es");
    database.setDatabaseName("broker_oficial");
    database.setPassword("20greatest_Showman22");
#else
    database.setPort(3306);
    database.setHostName("192.168.1.30");
    database.setDatabaseName("broker_test");
    database.setPassword("greatestShowman");
#endif
    opened = database.open();
    return opened;
};