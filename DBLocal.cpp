//
// Created by gabridc on 7/11/21.
//

#include "DBLocal.h"


DBLocal* DBLocal::db_ = nullptr;
QSqlDatabase DBLocal::database = QSqlDatabase();


/**
 * Static methods should be defined outside the class.
 */
DBLocal *DBLocal::GetInstance(void)
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if(db_ == nullptr){
        db_ = new DBLocal();
    }
    return db_;
}