//
// Created by gabridc on 7/11/21.
//

#ifndef BROKER_DBLOCAL_H
#define BROKER_DBLOCAL_H
#include "SQLManager.h"
#include "Wallet.h"
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
            auto wallet = query.value(0).toString();
            getWallet(wallet).print();
        }

    }

    Wallet getWallet(const QString& wallet)
    {
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT * FROM " + wallet);
        query.exec();

        std::cout << wallet.toStdString() << std::endl;
        std::cout << "------------" << std::endl;
        double amount = 0.0;
        double retired = 0.0;
        double available = 0.0;
        double invested = 0.0;
        while(query.next())
        {
            amount += query.value(1).toDouble();
            retired += query.value(2).toDouble();
            available += query.value(3).toDouble();
            //Calculo de lo que hay invertido en este momento
            invested += (query.value(3).toDouble() * query.value(5   ).toDouble());
        }

        return Wallet(wallet, amount, invested, available, retired);


    }

    std::tuple<bool, std::vector<Operation*>> getOperations(void)
    {
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT COUNT(*) FROM Operations");
        query.exec();
        bool result = false;
        while (query.next()) {
            result = query.value(0) > 0;
        }

        if(result)
        {
            std::vector<Operation *> operations;
            query.prepare("SELECT * FROM Operations");
            query.exec();
            while(query.next())
            {
                 operations.push_back(
                        new Operation(query.value(static_cast<int>(Operation::EN_OperationColumns_t::ID)).toInt(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIR1)).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIR2)).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIRA1AMOUNT)).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIRA1AMOUNTFIAT)).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIR2AMOUNT)).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIR2AMOUNTFIAT)).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMISION)).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMISIONFIAT)).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::STATUS)).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::DATE)).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMMENTS)).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::TYPE)).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::GANANCIA)).toDouble()
                        ));
            }
            return std::tuple<bool, std::vector<Operation *>>(result, operations);
        }
        else
        {
            return std::tuple<bool, std::vector<Operation *>>(result, {});
        }


    };


private:
    static DBLocal* db_;
    static QSqlDatabase database;
    QString databasePath;
};





#endif //BROKER_DBLOCAL_H
