//
// Created by gabridc on 3/11/21.
//

#include "SQLManager.h"
#include <unistd.h>

QSqlDatabase SQLManager::database = QSqlDatabase();
QString SQLManager::server = "None";

bool SQLManager::registerOperation(const int walletID1, const int walletID2, double pair1Amount, double pair1AmountFiat,
                       double pair2Amount, double pair2AmountFiat, QString feesCoin, double comision, double comisionFiat, QString& comments, QString& type,
                       QString& status, QString& date )
{

    /*
     * La comprobación de que la wallet origen tiene saldo suficiente debe hacerse antes de llamar a esta función
     */
    double ganancia = 0.0;
    QSqlQuery query = QSqlQuery(database);

    auto [r1, wallet1] = getWallet(walletID1);
    auto [r2, wallet2] = getWallet(walletID2);

    //Ambas wallets existen
    if(r1 and r2)
    {

        // Se resta la cantidad de monedas o fiat de la wallet origen. Las operaciones de staking/cashback se aplican cambios en wallets origen
        if(type == "Venta" || type == "Compra")
        {
            query.prepare("SELECT id, retired, available, fiat FROM WalletOperations WOP"
                          " WHERE WOP.wallet=:walletID");
            query.bindValue(":walletID", walletID1);
            query.exec();

            QSqlQuery query2 = QSqlQuery(database);
            auto pair1AmountAux = pair1Amount;


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

                //std::cout <<  id  <<" Retired: " <<  retired <<  " Available: " <<  available<< std::endl;


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
        else if(type == "Stacking" || type == "Cashback")
        {
            ganancia = pair2Amount * pair2AmountFiat;
        }
        // Nueva entrada con la cantidad de moneda en la wallet destino
        query.prepare("INSERT INTO WalletOperations(amount, retired, available, fiat,wallet)"
                      " VALUES (:amount, :retired, :available, :fiat,:walletID)");
        query.bindValue(":amount", pair2Amount);
        query.bindValue(":fiat", pair2AmountFiat);
        query.bindValue(":retired", 0.0);
        query.bindValue(":available", pair2Amount);
        query.bindValue(":walletID", walletID2);
        query.exec();

        query.prepare("INSERT INTO Operations(wallet1, wallet2, pair1Amount,pair1AmountFiat,pair2Amount,comision,comisionFiat,pair2AmountFiat,status,date,comments, type, ganancia)"
                       " VALUES(:wallet1,:wallet2,:pair1Amount,:pair1AmountFiat,:pair2Amount,:comision,:comisionFiat,:pair2AmountFiat,:status,:date,:comments,:type,:ganancia)");
        query.bindValue(":wallet1", walletID1);
        query.bindValue(":wallet2", walletID2);
        query.bindValue(":pair1Amount", pair1Amount);
        query.bindValue(":pair1AmountFiat", pair1AmountFiat);
        query.bindValue(":pair2Amount", pair2Amount);
        query.bindValue(":pair2AmountFiat", pair2AmountFiat);
        query.bindValue(":comision", comision);
        query.bindValue(":comisionFiat", comisionFiat);
        query.bindValue(":status", status);
        query.bindValue(":date", date);
        query.bindValue(":comments", comments);
        query.bindValue(":type", type);
        ganancia -= (comision * comisionFiat);
        query.bindValue(":ganancia", ganancia);

        std::cout << "operation data: " << walletID1 << " "  << walletID2 << " "  << pair1Amount  << " " << pair1AmountFiat << " " <<pair2Amount << " " << pair2AmountFiat << " " <<comision << " " <<comisionFiat  << " " <<status.toStdString() << " " << date.toStdString() << " " <<comments.toStdString() << " " << type.toStdString() << " " << ganancia << " "  << std::endl;

        return query.exec();
    }
    else
    {
        return false;
    }
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

std::tuple<bool, std::vector<Deposit*>> SQLManager::getDeposits(const uint32_t user)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT COUNT(*) FROM Deposits D"
                  " LEFT JOIN Wallets W on W.id = D.wallet"
                  " LEFT JOIN Coins C on C.id = W.coin"
                  " WHERE W.user=:user");

    query.bindValue(":user", user);
    query.exec();
    bool result = false;
    while (query.next()) {
        result = query.value(0) > 0;
    }

    std::cout << "RES: " << result << std::endl;

    if(result)
    {
        std::vector<Deposit*> deposits;
        query.prepare("SELECT C.name, W.exchange, W.user, D.* FROM Deposits D"
                      " LEFT JOIN Wallets W on W.id = D.wallet"
                      " LEFT JOIN Coins C on C.id = W.coin"
                      " WHERE W.user=:user");
        query.bindValue(":user", user);
        query.exec();
        while(query.next())
        {
            deposits.push_back(new Deposit(
                    query.value(0).toString(),
                    query.value(5).toDouble(),
                    query.value(6).toDouble(),
                    QDate::fromString(query.value(7).toString()),
                    query.value(1).toString(),
                    query.value(2).toString()
            ));
        }
        return std::tuple<bool, std::vector<Deposit*>>(true, deposits);
    }
    else
    {
        return std::tuple<bool, std::vector<Deposit*>>(false, {});
    }
}

void SQLManager::setWalletData(Wallet& wallet)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT C.name,U.username,W.exchange, WOP.* FROM WalletOperations WOP"
                  " LEFT JOIN Wallets W ON W.id = WOP.wallet"
                  " LEFT JOIN Coins C ON C.id = W.coin"
                  " LEFT JOIN Users U ON U.id = W.user"
                  " WHERE WOP.wallet =:walletID");
    query.bindValue(":walletID", wallet.getWalletID());
    query.exec();
    bool result = query.result()->handle().isValid();
    if(result)
    {
        bool first = true;
        double amount = 0.0;
        double invested = 0.0;
        while (query.next())
        {
            if(first)
            {
                wallet.setWalletID(query.value(4).toInt());
                wallet.setCoin(query.value(0).toString());
                wallet.setExchange(query.value(2).toString());
                wallet.setUser(query.value(1).toString());
            }
            amount += query.value(7).toDouble();
            invested += (query.value(7).toDouble() * query.value(8).toDouble());

            wallet.setAmount(amount);
            wallet.setInvested(invested);
            std::cout << "Wallet ID: " << wallet.getWalletID() << "Amount: " << wallet.getAmount() << std::endl;
        }
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

    query.prepare("SELECT * FROM Wallets"
                  " WHERE user=:user AND coin=:coin AND exchange=:exchange");
    query.bindValue(":user", user);
    query.bindValue(":coin", coin);
    query.bindValue(":exchange", exchange);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(result)
    {
        query.next();
        return query.value(0).toInt();
    }
    else
    {
        return 0;
    }

}

std::tuple<bool, Wallet*> SQLManager::getWallet(const uint32_t walletID)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT C.name,U.username,W.exchange, WOP.* FROM WalletOperations WOP"
                  " LEFT JOIN Wallets W ON W.id = WOP.wallet"
                  " LEFT JOIN Coins C ON C.id = W.coin"
                  " LEFT JOIN Users U ON U.id = W.user"
                  " WHERE WOP.wallet =:walletID");
    query.bindValue(":walletID", walletID);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(result)
    {
        auto wallet = new Wallet();
        bool first = true;
        double amount = 0.0;
        double invested = 0.0;
        while (query.next())
        {
            if(first)
            {
                wallet->setWalletID(query.value(4).toInt());
                wallet->setCoin(query.value(0).toString());
                wallet->setExchange(query.value(2).toString());
                wallet->setUser(query.value(1).toString());
            }
            amount += query.value(7).toDouble();
            invested += (query.value(7).toDouble() * query.value(8).toDouble());

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

std::tuple<bool, Wallet*> SQLManager::getWallet(const uint32_t user, const QString& exchange, const QString& coin)
{
    QSqlQuery query = QSqlQuery(database);
    std::cout << user << " " <<coin.toStdString() << " " <<exchange.toStdString() << std::endl;

    query.prepare("SELECT * FROM WalletOperations WOP"
                  " INNER JOIN Wallets W ON WOP.walletID = W.id"
                  " WHERE W.user=:user AND W.coin=:coin AND W.exchange=:exchange");
    query.bindValue(":user", user);
    query.bindValue(":coin", coin);
    query.bindValue(":exchange", exchange);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(result)
    {
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
    query.prepare("INSERT INTO Wallets(coin,exchange,user) "
                  " VALUES(:coin,:exchange,:user)");
    query.bindValue(":coin", coin);
    query.bindValue(":exchange", exchange);
    query.bindValue(":user", user);
    auto res = query.exec();
    if( res == true)
        return query.lastInsertId().toInt() ;
    else
        return 0;
}

std::tuple<bool, std::vector<Wallet*>> SQLManager::getWallets(void)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT C.name,U.username,  W.* FROM Wallets W"
                  " LEFT JOIN Coins C ON C.id = W.coin"
                  " LEFT JOIN Users U ON U.id = W.user");
    query.exec();
    bool result = query.result()->handle().isValid();
    if(result)
    {
        std::vector<Wallet*> wallets;
        while (query.next())
        {

            auto id = query.value(2).toInt();
            auto coin = query.value(0).toString();
            auto exchange = query.value(7).toString();
            auto user = query.value(1).toString();

            auto wallet = new Wallet(id, coin, exchange, user);
            wallets.push_back(wallet);
        }
        return std::tuple<bool, std::vector<Wallet*>> (true, wallets);
    }
    else
    {
        return std::tuple<bool, std::vector<Wallet*>> (false, {});
    }
}

std::tuple<bool, std::vector<Wallet*>> SQLManager::getWallets(const uint32_t userID)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT C.name, U.username,W.* FROM Wallets W"
                  " LEFT JOIN Coins C ON C.id = W.coin"
                  " LEFT JOIN Users U ON U.id = W.user"
                  " WHERE W.user=:userID");
    query.bindValue(":userID", userID);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(result)
    {
        std::vector<Wallet*> wallets;
        while (query.next())
        {

            auto id = query.value(2).toInt();
            auto coin = query.value(0).toString();
            auto exchange = query.value(4).toString();
            auto user = query.value(1).toString();

            auto wallet = new Wallet(id, coin, exchange, user);
            setWalletData(*wallet);
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

QList<std::tuple<uint32_t, QString, QString>> SQLManager::getCoins(void)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT * FROM Coins");
    query.exec();
    QList<std::tuple<uint32_t, QString, QString>> coins;
    while(query.next())
    {
        coins.push_back({query.value(0).toUInt(),  query.value(1).toString(), query.value(3).toString()});
    }

    return coins;
}


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
    query.prepare("SELECT * FROM Users WHERE username=:username" );
    query.bindValue(":username", username);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(result)
    {
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

QString SQLManager::getServer(void) const
{
    return server;
}


/*std::tuple<bool, std::vector<Operation*>> SQLManager::getOperations(void)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT C.name,C2.name,O.* FROM Operations O \
                        LEFT JOIN Wallets W1 on W1.id = O.wallet1 \
                        LEFT JOIN Coins C on C.id = W1.coin \
                        LEFT JOIN Wallets W2 on W2.id = O.wallet2 \
                        LEFT JOIN Coins C2 on C2.id = W2.coin \
                        WHERE W2.user=:user AND W1.user =:user \
                        ORDER BY id DESC");
    query.bindValue(":user", getUserID("gabridc"));
    query.exec();
    auto result = query.result()->handle().isValid();
    if(result)
    {
        std::vector<Operation *> operations;
        while(query.next())
        {
            operations.push_back(
                    new Operation(query.value(static_cast<int>(Operation::EN_OperationColumns_t::ID)+2).toInt(),
                                  query.value(0).toString(),
                                  query.value(1).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIRA1AMOUNT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIRA1AMOUNTFIAT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIR2AMOUNT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIR2AMOUNTFIAT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMISION)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMISIONFIAT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::STATUS)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::DATE)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMMENTS)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::TYPE)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::GANANCIA)+2).toDouble()
                    ));
        }
        return std::tuple<bool, std::vector<Operation *>>(result, operations);
    }
    else
    {
        return std::tuple<bool, std::vector<Operation *>>(result, {});
    }


};*/

std::tuple<bool, std::vector<Operation*>> SQLManager::getOperations(const uint32_t userID)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT C.name,C2.name,O.* FROM Operations O \
                        LEFT JOIN Wallets W1 on W1.id = O.wallet1 \
                        LEFT JOIN Coins C on C.id = W1.coin \
                        LEFT JOIN Wallets W2 on W2.id = O.wallet2 \
                        LEFT JOIN Coins C2 on C2.id = W2.coin \
                        WHERE W2.user=:user AND W1.user =:user \
                        ORDER BY id DESC");
    query.bindValue(":user", userID);
    query.exec();
    auto result = query.result()->handle().isValid();
    if(result)
    {
        std::vector<Operation *> operations;
        while(query.next())
        {
            operations.push_back(
                    new Operation(query.value(static_cast<int>(Operation::EN_OperationColumns_t::ID)+2).toInt(),
                                  query.value(0).toString(),
                                  query.value(1).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIRA1AMOUNT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIRA1AMOUNTFIAT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIR2AMOUNT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIR2AMOUNTFIAT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMISION)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMISIONFIAT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::STATUS)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::DATE)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMMENTS)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::TYPE)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::GANANCIA)+2).toDouble()
                    ));
        }
        return std::tuple<bool, std::vector<Operation *>>(result, operations);
    }
    else
    {
        return std::tuple<bool, std::vector<Operation *>>(result, {});
    }
}
std::tuple<bool, std::vector<Operation*>> SQLManager::getOperations(const uint32_t userID, const QString& exchange)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT C.name,C2.name,O.* FROM Operations O \
                        LEFT JOIN Wallets W1 on W1.id = O.wallet1 \
                        LEFT JOIN Coins C on C.id = W1.coin \
                        LEFT JOIN Wallets W2 on W2.id = O.wallet2 \
                        LEFT JOIN Coins C2 on C2.id = W2.coin \
                        WHERE W2.user=:user AND W1.user =:user AND W2.exchange=:exchange AND  W2.exchange=:exchange \
                        ORDER BY id DESC");
    query.bindValue(":user", userID);
    query.bindValue(":exchange", exchange);
    query.exec();
    auto result = query.result()->handle().isValid();
    if(result)
    {
        std::vector<Operation *> operations;
        while(query.next())
        {
            operations.push_back(
                    new Operation(query.value(static_cast<int>(Operation::EN_OperationColumns_t::ID)+2).toInt(),
                                  query.value(0).toString(),
                                  query.value(1).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIRA1AMOUNT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIRA1AMOUNTFIAT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIR2AMOUNT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::PAIR2AMOUNTFIAT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMISION)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMISIONFIAT)+2).toDouble(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::STATUS)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::DATE)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::COMMENTS)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::TYPE)+2).toString(),
                                  query.value(static_cast<int>(Operation::EN_OperationColumns_t::GANANCIA)+2).toDouble()
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

bool SQLManager::depositOperation(const int walletID, double amount, double amountFiat,  double fees, const QString& comments, QString& date)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("INSERT INTO Deposits(amount, fees, date, wallet)"
                  " VALUES (:amount, :fees, :date,:wallet)");
    query.bindValue(":amount", amount);
    query.bindValue(":date", date);
    query.bindValue(":fees", fees);
    query.bindValue(":wallet", walletID);
    if(query.exec())
    {
        query.prepare("INSERT INTO WalletOperations(amount, retired, available, fiat, wallet)"
                      " VALUES (:amount, :retired, :available, :fiat,:wallet)");
        query.bindValue(":amount", amount);
        query.bindValue(":fiat", amountFiat);
        query.bindValue(":retired", 0.0);
        query.bindValue(":available", amount);
        query.bindValue(":wallet", walletID);
        if(query.exec())
            return true;
    }
    else
        return false;



    return query.exec();
}



