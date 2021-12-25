//
// Created by gabridc on 3/11/21.
//

#include "SQLManager.h"
QSqlDatabase SQLManager::database = QSqlDatabase();

bool SQLManager::registerOperation(const int walletID1, const int walletID2, const QString& exchange, QString& pair1, QString& pair2, double pair1Amount, double pair1AmountFiat,
                       double pair2Amount, double pair2AmountFiat, double comision, double comisionFiat, QString& comments, QString& type,
                       QString& status, QString& date )
{

    /*
     * La comprobación de que la wallet origen tiene saldo suficiente debe hacerse antes de llamar a esta función
     */

    double ganancia = 0.0;
    QSqlQuery query = QSqlQuery(database);

    // Se resta la cantidad de monedas o fiat de la wallet origen. Las operaciones de staking no son compras/ventas
    // con wallets origen

    if(type == "Venta" || type == "Compra")
    {
        query.prepare("SELECT id, retired, available, fiat FROM WalletOperations WOP"
                      " WHERE WOP.walletID=:walletID");
        query.bindValue(":walletID", walletID1);
        query.exec();

        QSqlQuery query2 = QSqlQuery(database);
        auto pair1AmountAux = pair1Amount;



        std::cout << pair1Amount << std::endl;
        while(query.next() && pair1AmountAux != 0.0000)
        {
            auto id = query.value(0).toInt();
            auto available = query.value(2).toDouble();
            auto retired = query.value(1).toDouble();
            auto fiat = query.value(3).toDouble();

            if(available >= pair1AmountAux)
            {
                if(type == "Venta")
                    ganancia +=  ( pair1AmountAux * (pair1AmountFiat - fiat) );
                available -= pair1AmountAux;
                retired += pair1AmountAux;
                pair1AmountAux -= pair1AmountAux;
            }
            else
            {
                if(type == "Venta")
                    ganancia += (available * fiat);
                pair1AmountAux -= available;
                retired += available;
                available -= available;
            }

            std::cout <<  id  <<" Retired: " <<  retired <<  " Available: " <<  available<< std::endl;


            //Restar la cantidad de PAIR1
            query2.prepare("UPDATE WalletOperations"
                           " SET retired=:retired, available=:available"
                           " WHERE id=:id");
            query2.bindValue(":id", id);
            query2.bindValue(":retired", retired);
            query2.bindValue(":available", available);
            query2.exec();


        }
    }

    // Nueva entrada con la cantidad de moneda en la wallet destino
    query.prepare("INSERT INTO WalletOperations(amount, retired, available, fiat,walletID)"
                   " VALUES (:amount, :retired, :available, :fiat,:walletID)");
    query.bindValue(":amount", pair2Amount);
    query.bindValue(":fiat", pair2AmountFiat);
    query.bindValue(":retired", 0.0);
    query.bindValue(":available", pair2Amount);
    query.bindValue(":walletID", walletID2);
    query.exec();



    QSqlQuery query3 = QSqlQuery(database);
    query3.prepare("INSERT INTO Operations(pair1,pair2,pair1Amount,pair1AmountFiat,pair2Amount,comision,comisionFiat,pair2AmountFiat,status,date,comments, type, ganancia, exchpair1, exchPair2)"
                   " VALUES(:pair1,:pair2,:pair1Amount,:pair1AmountFiat,:pair2Amount,:comision,:comisionFiat,:pair2AmountFiat,:status,:date,:comments,:type,:ganancia, :exchpair1, :exchPair2)");
    query3.bindValue(":pair1", pair1);
    query3.bindValue(":pair2", pair2);
    query3.bindValue(":pair1Amount", pair1Amount);
    query3.bindValue(":pair1AmountFiat", pair1AmountFiat);
    query3.bindValue(":pair2Amount", pair2Amount);
    query3.bindValue(":pair2AmountFiat", pair2AmountFiat);
    query3.bindValue(":comision", comision);
    query3.bindValue(":comisionFiat", comisionFiat);
    query3.bindValue(":status", status);
    query3.bindValue(":date", date);
    query3.bindValue(":comments", comments);
    query3.bindValue(":type", type);
    ganancia -= (comision * comisionFiat);
    query3.bindValue(":ganancia", ganancia);
    query3.bindValue(":exchpair1", exchange);
    query3.bindValue(":exchPair2", exchange);
    return query3.exec();
}

std::tuple<bool, std::vector<Deposit*>> SQLManager::getDeposits(void)
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

std::tuple<bool, std::vector<Deposit*>> SQLManager::getDeposits(const QString& user)
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



std::tuple<bool, std::vector<Deposit*>> SQLManager::getDeposits(const QString& user, const QString& exchange)
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


int SQLManager::getWalletID(const uint32_t user, const QString& exchange, const QString& coin)
{
    std::cout << user << " " << coin.toStdString() << exchange.toStdString() << std::endl;
    QSqlQuery query = QSqlQuery(database);

    query.prepare("SELECT COUNT(*) FROM Wallets"
                  " WHERE user=:user AND coin=:coin AND exchange=:exchange");

    query.bindValue(":user", user);
    query.bindValue(":coin", coin);
    query.bindValue(":exchange", exchange);
    bool result = false;
    query.exec();
    query.next();
    std::cout << query.value(0).toInt() << std::endl;
    result = (query.value(0).toInt() == 1); // Existe la wallet
    if(result)
    {
        query.prepare("SELECT id FROM Wallets"
                      " WHERE user=:user AND coin=:coin AND exchange=:exchange");
        query.bindValue(":user", user);
        query.bindValue(":coin", coin);
        query.bindValue(":exchange", exchange);
        query.exec();
        query.next();
        return query.value(0).toInt();
    }
    else
    {
        return 0;
    }

}

std::tuple<bool, Wallet*> SQLManager::getWallet(const uint32_t user, const QString& exchange, const QString& coin)
{
    QSqlQuery query = QSqlQuery(database);
    std::cout << user << " " <<coin.toStdString() << " " <<exchange.toStdString() << std::endl;

    query.prepare("SELECT COUNT(*) FROM WalletOperations WOP"
                  " INNER JOIN Wallets W ON WOP.walletID = W.id"
                  " WHERE W.user=:user AND W.coin=:coin AND W.exchange=:exchange");
    query.bindValue(":user", user);
    query.bindValue(":coin", coin);
    query.bindValue(":exchange", exchange);
    query.exec();
    bool result = false;
    query.next();
    result = query.value(0).toInt() > 0;

    std::cout << query.value(0).toInt() << std::endl;

    if(result)
    {
        query.prepare("SELECT W.id,W.coin,W.user,W.exchange, WOP.available,WOP.fiat FROM WalletOperations WOP"
                      " INNER JOIN Wallets W ON WOP.walletID = W.id"
                      " WHERE W.user=:user AND W.coin=:coin AND W.exchange=:exchange");
        query.bindValue(":user", user);
        query.bindValue(":coin", coin);
        query.bindValue(":exchange", exchange);
        query.exec();
        auto wallet = new Wallet();
        bool first = true;
        double amount = 0.0;
        double invested = 0.0;
        while (query.next())
        {
            if(first)
            {
                wallet->setWalletID(query.value(0).toInt());
                wallet->setCoin(query.value(1).toString());
                wallet->setExchange(query.value(3).toString());
                wallet->setUser(query.value(2).toString());
            }


            amount += query.value(4).toDouble();
            invested += (query.value(4).toDouble() * query.value(5).toDouble());

            wallet->setAmount(amount);
            wallet->setInvested(invested);

        }
        return std::tuple<bool, Wallet*> (true, wallet);
    }
    else
    {
        return std::tuple<bool, Wallet*> (false, {});
    }
}

int SQLManager::addWallet(const QString& coin, double amount, const QString& exchange, const uint32_t user)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("INSERT INTO Wallets(coin,amount,exchange,user) "
                  " VALUES(:coin,:amount,:exchange,:user)");
    query.bindValue(":coin", coin);
    query.bindValue(":amount", amount);
    query.bindValue(":exchange", exchange);
    query.bindValue(":user", user);
    auto res = query.exec();
    if( res == true)
        return query.lastInsertId().toInt() ;
    else
        return 0;
}

std::tuple<bool, std::vector<Wallet*>> SQLManager::getWallets(const uint32_t userID)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT * FROM Wallets W"
                  " WHERE W.user=:userID");
    query.bindValue(":userID", userID);
    query.exec();
    bool result = false;
    query.next();
    result = query.value(0) > 0;
    if(result)
    {
        query.prepare("SELECT * FROM Wallets W"
                      " WHERE W.user=:userID");
        query.bindValue(":userID", userID);
        query.exec();
        std::vector<Wallet*> wallets;
        while (query.next())
        {

            auto id = query.value(0).toInt();
            auto coin = query.value(1).toString();
            auto exchange = query.value(5).toString();
            auto user = query.value(6).toString();

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



/* Wallet SQLManager::getWallet(const QString& wallet)
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


QList<std::tuple<uint32_t, QString>> SQLManager::getUsers(void)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT * FROM Users");
    query.exec();
    QList<std::tuple<uint32_t, QString>> users;
    while(query.next())
    {
        users.push_back({query.value(0).toUInt(),  query.value(1).toString()});
    }

    return users;
}

uint32_t SQLManager::getUserID(const QString& username)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT COUNT(*) FROM Users WHERE username=:username" );
    query.bindValue(":username", username);
    query.exec();

    bool result = false;
    while (query.next()) {
        result = query.value(0) > 0;
    }

    std::cout << result << std::endl;

    if(result)
    {

        query.prepare("SELECT COUNT(*) FROM Users WHERE username=:username" );
        query.bindValue(":username", username);
        query.exec();
        while(query.next())
        {
            std::cout << query.value(0).toUInt() << std::endl;
            return query.value(0).toUInt();
        }

    }
    else
    {
        return 0;
    }
}


std::tuple<bool, std::vector<Operation*>> SQLManager::getOperations(void)
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

std::tuple<bool, std::vector<Operation*>> SQLManager::getOperations(const QString& exchange)
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

double SQLManager::getInvested(const QString& user, const QString& exchange, const QString& wallet)
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

bool SQLManager::depositOperation(const int walletID, double pairAmount, double pairAmountFiat, const QString& comments, QString& date)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("INSERT INTO WalletOperations(amount, retired, available, fiat,walletID)"
                  " VALUES (:amount, :retired, :available, :fiat,:walletID)");
    query.bindValue(":amount", pairAmount);
    query.bindValue(":fiat", pairAmountFiat);
    query.bindValue(":retired", 0.0);
    query.bindValue(":available", pairAmount);
    query.bindValue(":walletID", walletID);
    return query.exec();
}
