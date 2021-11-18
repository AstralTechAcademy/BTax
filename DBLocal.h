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

    std::tuple<bool, std::vector<Deposit*>> getDeposits(void)
    {
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT COUNT(*) FROM Deposits");
        query.exec();
        bool result = false;
        while (query.next()) {
            result = query.value(0) > 0;
        }

        if(result)
        {
            std::vector<Deposit*> deposits;
            query.prepare("SELECT * FROM Deposits");
            query.exec();
            while(query.next())
            {
                deposits.push_back(new Deposit(
                        query.value(0).toString(),
                        query.value(1).toDouble(),
                        query.value(2).toDouble(),
                        query.value(3).toDouble(),
                        QDate::fromString("01/01/1999 00:00:00"),
                        query.value(5).toString(),
                        query.value(6).toString()
                ));
            }
            return std::tuple<bool, std::vector<Deposit*>>(false, deposits);
        }
        else
        {
            return std::tuple<bool, std::vector<Deposit*>>(false, {});
        }
    }

    std::tuple<bool, std::vector<Deposit*>> getDeposits(const QString& user)
    {
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT COUNT(*) FROM Deposits WHERE user=:user");
        query.bindValue(":user", user);
        query.exec();
        bool result = false;
        while (query.next()) {
            result = query.value(0) > 0;
        }

        if(result)
        {
            std::vector<Deposit*> deposits;
            query.prepare("SELECT * FROM Deposits WHERE user=:user");
            query.bindValue(":user", user);
            query.exec();
            while(query.next())
            {
                deposits.push_back(new Deposit(
                        query.value(0).toString(),
                        query.value(1).toDouble(),
                        query.value(2).toDouble(),
                        query.value(3).toDouble(),
                        QDate::fromString("01/01/1999 00:00:00"),
                        query.value(5).toString(),
                        query.value(6).toString()
                ));
            }
            return std::tuple<bool, std::vector<Deposit*>>(false, deposits);
        }
        else
        {
            return std::tuple<bool, std::vector<Deposit*>>(false, {});
        }
    }

    std::tuple<bool, std::vector<Deposit*>> getDeposits(const QString& user, const QString& exchange)
    {
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT COUNT(*) FROM Deposits WHERE user=:user AND exchange=:exchange");
        query.bindValue(":user", user);
        query.bindValue(":exchange", exchange);
        query.exec();
        bool result = false;
        while (query.next()) {
            result = query.value(0) > 0;
        }

        if(result)
        {
            std::vector<Deposit*> deposits;
            query.prepare("SELECT * FROM Deposits WHERE user=:user AND exchange=:exchange");
            query.bindValue(":user", user);
            query.bindValue(":exchange", exchange);
            query.exec();
            while(query.next())
            {
                deposits.push_back(new Deposit(
                        query.value(0).toString(),
                        query.value(1).toDouble(),
                        query.value(2).toDouble(),
                        query.value(3).toDouble(),
                        QDate::fromString("01/01/1999 00:00:00"),
                        query.value(5).toString(),
                        query.value(6).toString()
                ));
            }
            return std::tuple<bool, std::vector<Deposit*>>(false, deposits);
        }
        else
        {
            return std::tuple<bool, std::vector<Deposit*>>(false, {});
        }
    }

    std::tuple<bool, std::vector<Wallet*>> getWallets(const QString& user)
    {
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT * FROM Wallets W"
                      " WHERE W.user=:user");
        query.bindValue(":user", user);
        query.exec();
        bool result = false;
        query.next();
        result = query.value(0) > 0;
        if(result)
        {
            query.prepare("SELECT * FROM Wallets W"
                          " WHERE W.user=:user");
            query.bindValue(":user", user);
            query.exec();
            std::vector<Wallet*> wallets;
            while (query.next())
            {

                auto id = query.value(0).toInt();
                auto coin = query.value(1).toString();
                auto exchange = query.value(6).toString();
                auto user = query.value(7).toString();

                auto wallet = new Wallet(id, coin, exchange, user);

                QSqlQuery query2 = QSqlQuery(database);
                query2.prepare("SELECT WOP.* FROM WalletOperations WOP "
                              " WHERE WOP.walletID=:id");
                query2.bindValue(":id", id);
                query2.exec();
                double amount = 0.0;
                double invested = 0.0;
                while(query2.next())
                {
                    amount += query2.value(3).toDouble();
                    invested += (query2.value(3).toDouble() * query2.value(5).toDouble());
                }
                wallet->setAmount(amount);
                wallet->setInvested(invested);
                wallets.push_back(wallet);

            }

            return std::tuple<bool, std::vector<Wallet*>> (true, wallets);
        }
        else
        {
            return std::tuple<bool, std::vector<Wallet*>> (false, {});
        }
    }

   /* Wallet getWallet(const QString& wallet)
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

        return Wallet(query.value(0).toString(),
                      query.value(1).toDouble(),
                      query.value(2).toDouble(),
                      query.value(3).toDouble(),
                      query.value(4).toDouble(),
                      query.value(5).toString(),
                      query.value(6).toDouble(),
                      query.value(7).toString(),
                      query.value(8).toInt());


    }*/

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

    std::tuple<bool, std::vector<Operation*>> getOperations(const QString& exchange)
    {
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT COUNT(*) FROM Operations WHERE exchpair1=:exchange OR exchpair2=:exchange");
        query.bindValue(":exchange", exchange);
        query.exec();
        bool result = false;
        while (query.next()) {
            result = query.value(0) > 0;
        }

        if(result)
        {
            std::vector<Operation *> operations;
            query.prepare("SELECT * FROM Operations WHERE exchpair1=:exchange OR exchpair2=:exchange");
            query.bindValue(":exchange", exchange);
            query.exec();
            while(query.next())
            {
                std::cout << query.value(static_cast<int>(Operation::EN_OperationColumns_t::ID)).toInt() << std::endl;
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
    }

    double getInvested(const QString& user, const QString& exchange, const QString& wallet)
    {
        QSqlQuery query = QSqlQuery(database);
        query.prepare("SELECT COUNT(*) FROM WalletOperations WOP"
                      " INNER JOIN Wallets W ON WOP.walletID = W.id"
                      " WHERE W.user=:user AND W.coin =:coin AND W.exchange=:exchange");
        query.bindValue(":user", user);
        query.bindValue(":exchange", exchange);
        query.bindValue(":coin", wallet);
        query.exec();
        bool result = false;
        while (query.next()) {
            result = query.value(0) > 0;
        }

        if(result)
        {
            query.prepare("SELECT * FROM WalletOperations WOP"
                          " INNER JOIN Wallets W ON WOP.walletID = W.id"
                          " WHERE W.user=:user AND W.coin =:coin AND W.exchange=:exchange");
            query.bindValue(":user", user);
            query.bindValue(":exchange", exchange);
            query.bindValue(":coin", wallet);
            query.exec();
            double invested = 0.0;
            while(query.next())
            {
                double available = query.value(3).toDouble();
                double fiatPrice = query.value(5).toDouble();

                invested += (available * fiatPrice);


            }

            return invested;

        }

        return 0.0;
    }



private:
    static DBLocal* db_;
    static QSqlDatabase database;
    QString databasePath;
};





#endif //BROKER_DBLOCAL_H
