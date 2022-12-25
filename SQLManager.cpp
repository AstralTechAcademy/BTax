//
// Created by gabridc on 3/11/21.
//

#include "SQLManager.h"
#include <unistd.h>

QSqlDatabase SQLManager::database = QSqlDatabase();
QString SQLManager::server = "None";
QString SQLManager::databaseName = "None";

bool SQLManager::registerOperationNew(const std::vector<WalletOperation*> walletOperations,
                                      const WalletOperation::OperationData& data,
                                      std::vector<WalletOperation>& wOpsModified)
{

    /*
     * La comprobación de que la wallet origen tiene saldo suficiente debe hacerse antes de llamar a esta función
     */
    double ganancia = 0.0;
    QSqlQuery query = QSqlQuery(database);
    wOpsModified.clear();
    auto [r1, wallet1] = getWallet(data.walletID1);
    auto [r2, wallet2] = getWallet(data.walletID2);

    //Ambas wallets existen
    if(r1 and r2)
    {
        // Se resta la cantidad de monedas o fiat de la wallet origen. Las operaciones de staking/cashback se aplican cambios en wallets origen
        if(data.type == "Venta" || data.type == "Compra" )
        {
            auto index = 0;
            auto pair1AmountAux = data.pair1Amount;
            if(data.comisionFiat == data.pair1AmountFiat)
                pair1AmountAux += data.comision;

            if(data.type == "Compra")
                ganancia -= (data.comision * data.comisionFiat);

            while( index < walletOperations.size() && pair1AmountAux != 0.0000)  // Minetras haya compras y no se haya llegado a la cantidad vendida en la operacion
            {
                auto walletOp = walletOperations[index];
                auto id = walletOp->getID();
                auto available = walletOp->getAvailable();
                auto retired = walletOp->getRetired();
                auto fiat = walletOp->getFiatPrice();
                auto amount = 0.0;

                if(available >= pair1AmountAux)
                {
                    if(data.type == "Venta")
                        ganancia +=  ( pair1AmountAux * (data.pair1AmountFiat - fiat)); // Formula reviwed. OK.
                    available -= pair1AmountAux; // Formula reviwed. OK.
                    retired += pair1AmountAux; // Formula reviwed. OK.
                    amount = pair1AmountAux;
                    pair1AmountAux -= pair1AmountAux; // Formula reviwed. OK.
                }
                else // Se vende lo que resta porque si no hubiera cantidad sufiente se detecta en la funcion newOperation
                {
                    if(data.type == "Venta")
                        ganancia += (available * (data.pair1AmountFiat - fiat)); // Formula reviwed. OK.
                    amount = available;
                    pair1AmountAux -= available; // Formula reviwed. OK.
                    retired += available; // Formula reviwed. OK.
                    available -= available; // Formula reviwed. OK.
                }

                //Restar la cantidad de PAIR1
                QSqlQuery query2 = QSqlQuery(database);
                query2.prepare("UPDATE WalletOperations"
                               " SET retired=:retired, available=:available"
                               " WHERE id=:id");
                query2.bindValue(":id", id);
                query2.bindValue(":retired", retired);
                query2.bindValue(":available", available);
                query2.exec();
                query2.prepare("SELECT wallet, amount, retired, available, fiat, date FROM WalletOperations"
                               " WHERE id=:id");
                query2.bindValue(":id", id);
                query2.exec();
                query2.next();
                wOpsModified.push_back(WalletOperation(id, query2.value(0).toInt(),
                                                       query2.value(1).toDouble(), available,
                                                       retired, query2.value(4).toDouble(),
                                                       QDateTime::fromString(query2.value(5).toString())));
                index++;
            }
        }
        else if(data.type == "Stacking" || data.type == "Cashback" || data.type == "Airdrop" || data.type == "Earn") // Siempre genera una ganacia porque se considera como una venta desde 0€
        {
            ganancia = data.pair2Amount * data.pair2AmountFiat;
        }
        else if(data.type == "Transferencia")
        {
            auto index = 0;
            auto pair1AmountAux = data.pair1Amount;
            if(data.comisionFiat == data.pair1AmountFiat)
                pair1AmountAux += data.comision;

            while( index < walletOperations.size() && pair1AmountAux != 0.0000)  // Minetras haya compras y no se haya llegado a la cantidad vendida en la operacion
            {
                auto walletOp = walletOperations[index];
                auto id = walletOp->getID();
                auto available = walletOp->getAvailable();
                auto retired = walletOp->getRetired();
                auto fiat = walletOp->getFiatPrice();
                auto date = walletOp->getDate().toString();
                auto dateTimeUtc = walletOp->getDateTimeUtc();
                auto amount = 0.0;
                if(available >= pair1AmountAux)
                {
                    amount = pair1AmountAux;
                    available -= pair1AmountAux; // Formula reviwed. OK.
                    retired += pair1AmountAux; // Formula reviwed. OK.
                    pair1AmountAux -= pair1AmountAux; // Formula reviwed. OK.
                }
                else // Se vende lo que resta porque si no hubiera cantidad sufiente se detecta en la funcion newOperation
                {
                    amount = available;
                    pair1AmountAux -= available; // Formula reviwed. OK.
                    retired += available; // Formula reviwed. OK.
                    available -= available; // Formula reviwed. OK.
                }

                ganancia = 0.0; // Formula reviwed. OK.

                QDateTime time = QDateTime::fromString(data.date);
 
                //Restar la cantidad de PAIR1
                QSqlQuery query2 = QSqlQuery(database);
                query2.prepare("UPDATE WalletOperations"
                               " SET retired=:retired, available=:available"
                               " WHERE id=:id");
                query2.bindValue(":id", id);
                query2.bindValue(":retired", retired);
                query2.bindValue(":available", available);
                query2.exec();

                std::cout << "BYE: " << id << retired << available << std::endl;

                query2.prepare("SELECT wallet, amount, retired, available, fiat, date FROM WalletOperations"
                               " WHERE id=:id");
                query2.bindValue(":id", id);
                query2.exec();
                query2.next();
                wOpsModified.push_back(WalletOperation(id, query2.value(0).toInt(),
                                                       query2.value(1).toDouble(), available,
                                                       retired, query2.value(4).toDouble(),
                                                       QDateTime::fromString(query2.value(5).toString())));
                
                // Nueva entrada con la cantidad de moneda en la wallet destino
                query.prepare("INSERT INTO WalletOperations(amount, retired, available, fiat,wallet, date, datetimeUTC)"
                              " VALUES (:amount, :retired, :available, :fiat,:walletID, :date, :datetimeUTC)");
                query.bindValue(":amount", amount);
                query.bindValue(":fiat", fiat);
                query.bindValue(":retired", 0.0);
                query.bindValue(":available", amount);
                query.bindValue(":walletID", data.walletID2);
                query.bindValue(":date", date);
                query.bindValue(":datetimeUTC", dateTimeUtc);
                query.exec();
                
                index++;

            }
        }

        QDateTime time = QDateTime::fromString(data.date);
        if (data.type != "Transferencia")
        {
            // Nueva entrada con la cantidad de moneda en la wallet destino
            query.prepare("INSERT INTO WalletOperations(amount, retired, available, fiat,wallet, date, datetimeUTC)"
                          " VALUES (:amount, :retired, :available, :fiat,:walletID, :date, :datetimeUTC)");
            query.bindValue(":amount", data.pair2Amount);
            query.bindValue(":fiat", data.pair2AmountFiat);
            query.bindValue(":retired", 0.0);
            query.bindValue(":available", data.pair2Amount);
            query.bindValue(":walletID", data.walletID2);
            query.bindValue(":date", data.date);
            query.bindValue(":datetimeUTC", dateTimeToUTC0(time, wallet2->getExchange()));
            query.exec();
        }

        std::cout << "INSERT INTO OPERATIONS(wallet1, wallet2, pair1Amount,pair1AmountFiat,pair2Amount,comision,comisionFiat,pair2AmountFiat,status,date,comments, type, ganancia)"
                     " VALUES("
                  << data.walletID1 << ","
                  << data.walletID2 << ","
                  << data.pair1Amount << ","
                  << data.pair1AmountFiat << ","
                  << data.pair2Amount << ","
                  << data.pair2AmountFiat << ","
                  << data.comision << ","
                  << data.comisionFiat << ","
                  << data.status.toStdString() << ","
                  << data.date.toStdString() << ","
                  << data.type.toStdString() << ","
                  << ganancia << std::endl;

        query.prepare("INSERT INTO Operations(wallet1, wallet2, pair1Amount,pair1AmountFiat,pair2Amount,comision,comisionFiat,pair2AmountFiat,status,date,comments, type, ganancia)"
                      " VALUES(:wallet1,:wallet2,:pair1Amount,:pair1AmountFiat,:pair2Amount,:comision,:comisionFiat,:pair2AmountFiat,:status,:date,:comments,:type,:ganancia)");
        query.bindValue(":wallet1", data.walletID1);
        query.bindValue(":wallet2", data.walletID2);
        query.bindValue(":pair1Amount", data.pair1Amount);
        query.bindValue(":pair1AmountFiat", data.pair1AmountFiat);
        query.bindValue(":pair2Amount", data.pair2Amount);
        query.bindValue(":pair2AmountFiat", data.pair2AmountFiat);
        query.bindValue(":comision", data.comision);
        query.bindValue(":comisionFiat", data.comisionFiat);
        query.bindValue(":status", data.status);
        query.bindValue(":date", data.date);
        query.bindValue(":comments", data.comments);
        query.bindValue(":type", data.type);
        query.bindValue(":ganancia", ganancia);
        return query.exec();
    }
    else
    {
        return false;
    }
}

bool SQLManager::registerAsset(const QString& type, const QString& name, const QString& color)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("INSERT INTO Coins(name, type, color)"
                  " VALUES (:name, :type, :color)");
    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":color", color);
    return query.exec();
}

bool SQLManager::registerUser(const QString& username)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("INSERT INTO Users(username) VALUES (:username)");
    query.bindValue(":username", username);
    return query.exec();
}


std::optional<std::vector<Deposit*>> SQLManager::getDeposits(void)
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
                    query.value(2).toDouble(),
                    query.value(3).toDouble(),
                    QDateTime::fromString(query.value(4).toString()),
                    query.value(1).toString()
            ));
        }
        return std::vector<Deposit*>(deposits);
    }
    else
    {
        return std::nullopt;
    }
}

std::optional<std::vector<Deposit*>> SQLManager::getDeposits(const uint32_t user)
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
                    QDateTime::fromString(query.value(7).toString()),
                    query.value(4).toString()
            ));
        }
        return std::vector<Deposit*>(deposits);
    }
    else
    {
        return std::nullopt;
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
                    query.value(2).toDouble(),
                    query.value(3).toDouble(),
                    QDateTime::fromString(query.value(4).toString()),
                    query.value(1).toString()
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
    query.prepare("SELECT C.name,U.username,W.exchange, WOP.* FROM Wallets W"
                  " LEFT JOIN WalletOperations WOP ON WOP.wallet = W.id"
                  " LEFT JOIN Coins C ON C.id = W.coin"
                  " LEFT JOIN Users U ON U.id = W.user"
                  " WHERE W.id =:walletID");
    query.bindValue(":walletID", walletID);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(result)
    {
        result = false;
        auto wallet = new Wallet();
        bool first = true;
        double amount = 0.0;
        double invested = 0.0;
        while (query.next())
        {
            result = true;
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
        return std::tuple<bool, Wallet*> (result, wallet);
    }
    else
    {
        return std::tuple<bool, Wallet*> (false, {});
    }
}

std::tuple<bool, Wallet*> SQLManager::getWallet(const uint32_t user, const QString& exchange, const QString& coin)
{
    QSqlQuery query = QSqlQuery(database);
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

std::optional<std::vector<const WalletOperation*>> SQLManager::getWalletOperations(const QString& walletID)
{
    QSqlQuery query = QSqlQuery(database);

    query.prepare("SELECT WOP.id, WOP.wallet, WOP.amount, WOP.retired, WOP.available, WOP.fiat, WOP.date, "
                  " C.name, W.exchange, W.user, WOP.datetimeUTC FROM WalletOperations WOP "
                  " LEFT JOIN Wallets W ON W.id = WOP.wallet"
                  " LEFT JOIN Coins C ON C.id = W.coin"
                  " WHERE WOP.wallet =:walletID");
    query.bindValue(":walletID", walletID);
    query.exec();
    bool result = query.result()->handle().isValid();

    if(result)
    {
        std::vector<const WalletOperation*> walletOps;

        while (query.next())
        {
            auto exchange = query.value(8).toString();
            QDateTime dateTimeUTC;
            if (query.value(10).isNull())
                dateTimeUTC =  QDateTime::fromString(updateDateTimeUTCFromQTFormat("WalletOperations", query.value(0).toString(), cnvDBStr2DateTime(query.value(6).toString()), exchange));
            else    
                dateTimeUTC = DatetimeUTCStrToDatetime(query.value(10).toString());

            auto wop= new WalletOperation(
                    query.value(0).toInt(),
                    query.value(1).toInt(),
                    query.value(7).toString(),
                    query.value(8).toString(),
                    query.value(9).toString(),
                    query.value(2).toDouble(),
                    query.value(4).toDouble(),
                    query.value(3).toDouble(),
                    query.value(5).toDouble(),
                    QDateTime::fromString(query.value(6).toString()),
                    dateTimeUTC);
            walletOps.push_back(wop);
        }

        if(walletOps.size() == 0)
            return std::nullopt;
        else
            return walletOps;
    }
    else
    {
        return std::nullopt;
    }
}

int SQLManager::addWallet(const QString& coin, const QString& exchange, const uint32_t user)
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
    query.prepare("SELECT C.name,U.username, W.*, C.id, C.type, C.color FROM Wallets W"
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
            auto coinName = query.value(0).toString();
            auto exchange = query.value(4).toString();
            auto user = query.value(1).toString();
            auto coinID = query.value(7).toInt();
            auto color = query.value(8).toString();
            auto type = query.value(9).toString();

            auto ws = getWalletOperations(QString::number(id));
            Wallet* wallet;
            if (ws == std::nullopt)
                wallet = new Wallet(id, coinName, exchange, user, new Coin(coinID, coinName, color, type), std::vector<const WalletOperation*>());
            else
                wallet = new Wallet(id, coinName, exchange, user, new Coin(coinID, coinName, color, type), ws.value());

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
    std::cout << "Hey";
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT C.name, U.username,W.*, C.id, C.type, C.color FROM Wallets W"
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
            auto coinName = query.value(0).toString();
            auto exchange = query.value(4).toString();
            auto user = query.value(1).toString();
            auto coinID = query.value(7).toInt();
            auto color = query.value(9).toString();
            auto type = query.value(8).toString();

            auto ws = getWalletOperations(QString::number(id));
            Wallet* wallet;
            if (ws == std::nullopt)
                wallet = new Wallet(id, coinName, exchange, user, new Coin(coinID, coinName, color, type), std::vector<const WalletOperation*>());
            else
                wallet = new Wallet(id, coinName, exchange, user, new Coin(coinID, coinName, color, type), ws.value());

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

QString SQLManager::getUsername(uint32_t userid)
{
    QSqlQuery query = QSqlQuery(database);

    query.prepare("SELECT U.username"
                  " FROM Users U"
                  " WHERE U.id=:userid");
    query.bindValue(":userid", userid);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(!result)
        return 0;

    while (query.next())
    {
        return query.value(0).toString();
    }
}

std::optional<Wallet> SQLManager::getWallets(const uint32_t userID, const QString& coin, const QString exchange)
{
    QSqlQuery query = QSqlQuery(database);

    query.prepare("SELECT C.name, U.username,W.*, C.id, C.type, C.color"
                  " FROM Wallets W"
                  " LEFT JOIN Coins C ON C.id = W.coin"
                  " LEFT JOIN Users U ON U.id = W.user"
                  " WHERE W.user=:user AND W.coin =:coin AND W.exchange=:exchange");
    query.bindValue(":exchange", exchange);
    query.bindValue(":user", userID);
    query.bindValue(":coin", coin);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(!result)
        return std::nullopt;

    while (query.next())
    {

        auto id = query.value(2).toInt();
        auto coinName = query.value(0).toString();
        auto exchange = query.value(4).toString();
        auto user = query.value(1).toString();
        auto coinID = query.value(7).toInt();
        auto color = query.value(9).toString();
        auto type = query.value(8).toString();

        auto ws = getWalletOperations(QString::number(id));
        Wallet* wallet;
        if (ws == std::nullopt)
            wallet = new Wallet(id, coinName, exchange, user, new Coin(coinID, coinName, color, type), std::vector<const WalletOperation*>());
        else
            wallet = new Wallet(id, coinName, exchange, user, new Coin(coinID, coinName, color, type), ws.value());

        setWalletData(*wallet);
        return *wallet;
    }

    return std::nullopt;


}

std::optional<std::vector<WalletOperation*>> SQLManager::getWalletsOps(const uint32_t userID, const QString& coin, const QString exchange)
{
    QSqlQuery query = QSqlQuery(database);
    if(exchange == "")
    {
        query.prepare("SELECT WOP.wallet, WOP.amount, WOP.retired, WOP.available, WOP.fiat, WOP.date, W.coin, W.exchange, W.user, WOP.id, WOP.datetimeUTC"
                      " FROM WalletOperations WOP"
                      " INNER JOIN Wallets W ON WOP.wallet = W.id"
                      " WHERE W.user=:user AND W.coin =:coin");
    }
    else
    {
        query.prepare("SELECT WOP.wallet, WOP.amount, WOP.retired, WOP.available, WOP.fiat, WOP.date, W.coin, W.exchange, W.user, WOP.id, WOP.datetimeUTC"
                      " FROM WalletOperations WOP"
                      " INNER JOIN Wallets W ON WOP.wallet = W.id"
                      " WHERE W.user=:user AND W.coin =:coin AND W.exchange=:exchange");
        query.bindValue(":exchange", exchange);
    }

    query.bindValue(":user", userID);
    query.bindValue(":coin", coin);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(!result)
        return std::nullopt;

    std::vector<WalletOperation*> wallets;
    while (query.next())
    {
        auto id = query.value(9).toInt();
        auto walletID = query.value(0).toInt();
        auto amount = query.value(1).toDouble();
        auto available = query.value(3).toDouble();
        auto retired = query.value(2).toDouble();
        auto fiat = query.value(4).toDouble();
        auto coin = query.value(6).toString();
        auto date = QDateTime::fromString(query.value(5).toString());
        auto datetimeUtc = DatetimeUTCStrToDatetime(query.value(10).toString());
        auto exchange = query.value(7).toString();
        auto user = query.value(8).toString();

        auto wallet = new WalletOperation(id, walletID, coin, exchange, user, amount, available, retired, fiat, date, datetimeUtc);
        wallets.push_back(wallet);
    }

    return wallets;
}

std::optional<std::vector<Wallet*>> SQLManager::getCryptoWallets(const uint32_t userID)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT C.name,U.username, W.*, C.id, C.type, C.color FROM Wallets W"
                  " LEFT JOIN Coins C ON C.id = W.coin"
                  " LEFT JOIN Users U ON U.id = W.user"
                  " WHERE W.user=:user AND C.type = \"crypto\"");

    query.bindValue(":user", userID);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(result)
    {
        std::vector<Wallet*> wallets;
        while (query.next())
        {
            auto id = query.value(2).toInt();
            auto coinName = query.value(0).toString();
            auto exchange = query.value(4).toString();
            auto user = query.value(1).toString();
            auto coinID = query.value(7).toInt();
            auto color = query.value(9).toString();
            auto type = query.value(8).toString();

            auto ws = getWalletOperations(QString::number(id));
            Wallet* wallet;
            if (ws == std::nullopt)
                wallet = new Wallet(id, coinName, exchange, user, new Coin(coinID, coinName, color, type), std::vector<const WalletOperation*>());
            else
                wallet = new Wallet(id, coinName, exchange, user, new Coin(coinID, coinName, color, type), ws.value());

            wallet->print();
            setWalletData(*wallet);
            wallets.push_back(wallet);
        }
        return wallets;
    }
    else
    {
        std::cout << "Wallets not found" << std::endl;
        return std::nullopt;
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

QList<std::tuple<uint32_t, QString, QString, QString>> SQLManager::getCoins(void)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT * FROM Coins");
    query.exec();
    QList<std::tuple<uint32_t, QString, QString, QString>> coins;
    while(query.next())
    {
        coins.push_back({query.value(0).toUInt(),  query.value(1).toString(), query.value(3).toString(), query.value(2).toString()});
    }

    return coins;
}

std::optional<std::tuple<uint32_t, QString, QString, QString>> SQLManager::getCoin(const QString& coin)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT id, name, type, color FROM Coins WHERE name=:coin");
    query.bindValue(":coin", coin);
    query.exec();
    bool result = query.result()->handle().isValid();
    if(!result)
        return std::nullopt;

    while(query.next())
        return std::tuple(query.value(0).toUInt(),  query.value(1).toString(), query.value(3).toString(), query.value(2).toString());
}

QList<std::tuple<uint32_t, QString>> SQLManager::getAssetTypes(void)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT * FROM AssetType");
    query.exec();
    QList<std::tuple<uint32_t, QString>> assets;
    while(query.next())
        assets.push_back({query.value(0).toUInt(),  query.value(1).toString()});

    return assets;
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

QString SQLManager::getDatabase(void) const
{
    return databaseName;
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

std::tuple<bool, std::vector<Operation*>> SQLManager::getOperations(const QString& walletID)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("SELECT C.name,C2.name,O.* FROM Operations O \
                        LEFT JOIN Wallets W1 on W1.id = O.wallet1 \
                        LEFT JOIN Coins C on C.id = W1.coin \
                        LEFT JOIN Wallets W2 on W2.id = O.wallet2 \
                        LEFT JOIN Coins C2 on C2.id = W2.coin \
                        WHERE O.wallet1=:wallet OR O.wallet2 =:wallet \
                        ORDER BY id DESC");
    query.bindValue(":wallet", walletID);
    query.exec();
    return processGetOperations(query);
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
    return processGetOperations(query);
    
}

std::tuple<bool, std::vector<Operation*>> SQLManager::processGetOperations(QSqlQuery& query)
{
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

bool SQLManager::depositOperation(const int walletID, const QString exchange, double amount, double amountFiat,  double fees, const QString& comments, QString& date)
{
    QSqlQuery query = QSqlQuery(database);
    std::cout << "TRACE 1" << std::endl;

    query.prepare("INSERT INTO Deposits(amount, fees, date, wallet)"
                  " VALUES (:amount, :fees, :date,:wallet)");
    query.bindValue(":amount", amount);
    query.bindValue(":date", date);
    query.bindValue(":fees", fees);
    query.bindValue(":wallet", walletID);
    if(query.exec())
    {
        std::cout << "TRACE 2" << std::endl;
        QDateTime time = QDateTime::fromString(date);
        query.prepare("INSERT INTO WalletOperations(amount, retired, available, fiat, wallet, date, datetimeUTC)"
                      " VALUES (:amount, :retired, :available, :fiat,:wallet, :date, :datetimeUTC)");
        query.bindValue(":amount", amount);
        query.bindValue(":fiat", amountFiat);
        query.bindValue(":retired", 0.0);
        query.bindValue(":available", amount);
        query.bindValue(":wallet", walletID);
        query.bindValue(":date", date);
        query.bindValue(":datetimeUTC", dateTimeToUTC0(time, exchange));
        std::cout << "TRACE 3" << std::endl;
        if(query.exec())
            return true;
    }
    else
        return false;



    return query.exec();
}

QDateTime SQLManager::DatetimeUTCStrToDatetime(QString timeUtc) const
{
    auto date = timeUtc.split("T")[0];
    auto time = timeUtc.split("T")[1];

    auto day = date.split("-")[2];
    auto month = date.split("-")[1];
    auto year = date.split("-")[0];

    auto hour = time.split(":")[0];
    auto minute = time.split(":")[1];
    auto sec = time.split(":")[2];

    return QDateTime(QDate(year.toInt(), month.toInt(), day.toInt()), QTime(hour.toInt(), minute.toInt(), sec.toInt()));
}

QString SQLManager::updateDateTimeUTCFromQTFormat(QString table, QString id, QDateTime time, QString exchange)
{
    QSqlQuery query = QSqlQuery(database);
    query.prepare("UPDATE WalletOperations"
                  " SET dateTimeUTC=:time"
                  " WHERE id=:id");
    query.bindValue(":id", id);
    query.bindValue(":time", dateTimeToUTC0(time, exchange));
    query.exec();

    return dateTimeToUTC0(time, exchange);
}

Operation* SQLManager::getLastOperation(void) const
{
    QSqlQuery query = QSqlQuery(database);

    std::cout << "File:SQLManager.cpp Function: getLastOperation" << std::endl;

    query.prepare("SELECT OP.id, C1.name, C2.name, pair1Amount, pair1AmountFiat, pair2Amount, pair2AmountFiat, comision, comisionFiat, "
                  "status, date, OP.type, ganancia"
                  " FROM Operations OP"
                  " INNER JOIN Wallets W1 ON W1.id = wallet1"
                  " INNER JOIN Wallets W2 ON W2.id = wallet2"
                  " INNER JOIN Coins C1 ON C1.id = W1.coin"
                  " INNER JOIN Coins C2 ON C2.id = W2.coin"
                  " ORDER BY id DESC LIMIT 1");

    query.exec();
    query.next();

    auto op = new Operation(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toDouble(), query.value(4).toDouble(),
                           query.value(5).toDouble(),query.value(6).toDouble(), query.value(7).toDouble(), query.value(8).toDouble(),
                           query.value(9).toString(), query.value(10).toString(), "", query.value(11).toString(), query.value(12).toDouble());

    op->print();
    return op;
}

std::vector<WalletOperation*>  SQLManager::getLastNWalletOperation(int limit) const
{
    QSqlQuery query = QSqlQuery(database);
    std::vector<WalletOperation*> wOps;
    std::cout << "File:SQLManager.cpp Function: getLastNWalletOperation" << std::endl;

    query.prepare("SELECT WOP.id, C1.name, W1.exchange, U.username, WOP.wallet, WOP.amount, WOP.retired, WOP.available, WOP.fiat, WOP.date, WOP.datetimeUTC FROM WalletOperations WOP"
                  " INNER JOIN Wallets W1 ON W1.id = WOP.wallet"
                  " INNER JOIN Users U ON U.id = W1.user"
                  " INNER JOIN Coins C1 ON C1.id = W1.coin"
                  " ORDER BY id desc limit " + QString::number(limit));
    query.exec();

    while(query.next())
    {
        auto opW = new WalletOperation(query.value(0).toInt(), query.value(4).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toString(),
                                       query.value(5).toDouble(), query.value(7).toDouble(), query.value(6).toDouble(), query.value(8).toDouble(),
                                       QDateTime::fromString(query.value(9).toString()),
                                       DatetimeUTCStrToDatetime(query.value(10).toString()));
        opW->print();
        wOps.push_back(opW);
    }
    return wOps;
}



