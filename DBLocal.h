//
// Created by gabridc on 7/11/21.
//

#ifndef BROKER_DBLOCAL_H
#define BROKER_DBLOCAL_H
#include "SQLManager.h"
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
    bool createDatabase(void)
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


    bool openDatabase(void)
    {
        bool opened = false;
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databasePath);
        opened = database.open();
        return opened;
    };

    void getWallets(void)
    {
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT * FROM Wallets");
        query.exec();

        while(query.next())
        {
            std::cout << query.value(0).toString().toStdString() << std::endl;
        }

    }

private:
    static DBLocal* db_;
    static QSqlDatabase database;
    QString databasePath;
};





#endif //BROKER_DBLOCAL_H
