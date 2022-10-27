//
// Created by gabridc on 6/12/21.
//

#include "DBRemote.h"
#include <future>
#include "DBCredential_NotPush.h"

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
    server = SERVER;
    databaseName = DATABASENAME;
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
    database.setUserName(USERNAME);
    database.setPort(PORT);
    database.setHostName(SERVER);
    database.setDatabaseName(DATABASENAME);
    database.setPassword(PASSWORD);
    server = SERVER;
    databaseName = DATABASENAME;




    opened = database.open();
    return opened;
};