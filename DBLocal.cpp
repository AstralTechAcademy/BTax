//
// Created by gabridc on 7/11/21.
//

#include "DBLocal.h"


DBLocal* DBLocal::db_ = nullptr;



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


bool DBLocal::createDatabase(void)
{
    bool created = false;
    QDir dir;
    QString kernel = QSysInfo::kernelType();
    if(kernel == "linux")
    {
        databasePath = LinuxDatabasePath;
        if(QFile(LinuxDatabasePath).exists() == false)
        {
            QFile db(LinuxDatabasePath);
            db.open(QIODevice::WriteOnly);
            db.close();
            created = true;
        }
        else
        {
            //BD ya creada
            std::cout << "File: DBLocal.cpp Function: createDatabase La DB ya existe" << std::endl;
            created = true;
        }
    }

    return created;
};


bool DBLocal::openDatabase(void)
{
    bool opened = false;
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databasePath);
    opened = database.open();
    return opened;
};