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
                          " WHERE WOP.wallet=:walletID"); // Obtiene todas las compras en esa wallet
            query.bindValue(":walletID", walletID1);
            query.exec();

            auto pair1AmountAux = pair1Amount;
            while(query.next() && pair1AmountAux != 0.0000)  // Minetras haya compras y no se haya llegado a la cantidad vendida en la operacion
            {
                auto id = query.value(0).toInt();
                auto available = query.value(2).toDouble();
                auto retired = query.value(1).toDouble();
                auto fiat = query.value(3).toDouble();

                if(available >= pair1AmountAux)
                {
                    if(type == "Venta")
                        ganancia +=  ( pair1AmountAux * (pair1AmountFiat - fiat)); // Formula reviwed. OK.
                    available -= pair1AmountAux; // Formula reviwed. OK.
                    retired += pair1AmountAux; // Formula reviwed. OK.
                    pair1AmountAux -= pair1AmountAux; // Formula reviwed. OK.
                }
                else // Se vende lo que resta porque si no hubiera cantidad sufiente se detecta en la funcion newOperation
                {
                    if(type == "Venta")
                        ganancia += (available * fiat); // Formula reviwed. OK.
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
            }
        }
        else if(type == "Stacking" || type == "Cashback" || type == "Airdrop" || "Earn") // Siempre genera una ganacia porque se considera como una venta desde 0€
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

        std::cout << "INSERT INTO OPERATIONS(wallet1, wallet2, pair1Amount,pair1AmountFiat,pair2Amount,comision,comisionFiat,pair2AmountFiat,status,date,comments, type, ganancia)"
                     " VALUES("
                << walletID1 << ","
                << walletID2 << ","
                << pair1Amount << ","
                << pair1AmountFiat << ","
                << pair2Amount << ","
                << pair2AmountFiat << ","
                << comision << ","
                << comisionFiat << ","
                << status.toStdString() << ","
                << date.toStdString() << ","
                << type.toStdString() << ","
                << ganancia << std::endl;

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
        return query.exec();
    }
    else
    {
        return false;
    }
}

bool SQLManager::registerOperationNew(const std::vector<WalletOperation*> walletOperations, const WalletOperation::OperationData& data)
{

    /*
     * La comprobación de que la wallet origen tiene saldo suficiente debe hacerse antes de llamar a esta función
     */
    double ganancia = 0.0;
    QSqlQuery query = QSqlQuery(database);

    auto [r1, wallet1] = getWallet(data.walletID1);
    auto [r2, wallet2] = getWallet(data.walletID2);

    //Ambas wallets existen
    if(r1 and r2)
    {
        // Se resta la cantidad de monedas o fiat de la wallet origen. Las operaciones de staking/cashback se aplican cambios en wallets origen
        if(data.type == "Venta" || data.type == "Compra")
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

                if(available >= pair1AmountAux)
                {
                    if(data.type == "Venta")
                        ganancia +=  ( pair1AmountAux * (data.pair1AmountFiat - fiat)); // Formula reviwed. OK.
                    available -= pair1AmountAux; // Formula reviwed. OK.
                    retired += pair1AmountAux; // Formula reviwed. OK.
                    pair1AmountAux -= pair1AmountAux; // Formula reviwed. OK.
                }
                else // Se vende lo que resta porque si no hubiera cantidad sufiente se detecta en la funcion newOperation
                {
                    if(data.type == "Venta")
                        ganancia += (available * (data.pair1AmountFiat - fiat)); // Formula reviwed. OK.
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
                index++;
            }
        }
        else if(data.type == "Stacking" || data.type == "Cashback" || data.type == "Airdrop" || "Earn") // Siempre genera una ganacia porque se considera como una venta desde 0€
        {
            ganancia = data.pair2Amount * data.pair2AmountFiat;
        }
        QDateTime time = QDateTime::fromString(data.date);

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
        ganancia -= (data.comision * data.comisionFiat);
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
                    QDateTime::fromString( query.value(6).toString()));
            wop->print();
            walletOps.push_back(wop);

            auto exchange = query.value(8).toString();
            if (query.value(10).isNull())
                updateDateTimeUTCFromQTFormat("WalletOperations", query.value(0).toString(), QDateTime::fromString( query.value(6).toString()), exchange);

        }
        return walletOps;
    }
    else
    {
        return std::nullopt;
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

std::optional<Wallet> SQLManager::getWallets(const uint32_t userID, const QString& coin, const QString exchange)
{
    QSqlQuery query = QSqlQuery(database);

    query.prepare("SELECT C.name, U.username,W.*, C.id, C.type, C.color"
                  " FROM Walllets W"
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
        query.prepare("SELECT WOP.wallet, WOP.amount, WOP.retired, WOP.available, WOP.fiat, WOP.date, W.coin, W.exchange, W.user, WOP.id"
                      " FROM WalletOperations WOP"
                      " INNER JOIN Wallets W ON WOP.wallet = W.id"
                      " WHERE W.user=:user AND W.coin =:coin");
    }
    else
    {
        query.prepare("SELECT WOP.wallet, WOP.amount, WOP.retired, WOP.available, WOP.fiat, WOP.date, W.coin, W.exchange, W.user, WOP.id"
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
        auto fiat = query.value(4).toInt();
        auto coin = query.value(6).toString();
        auto date = query.value(5).toString();
        auto exchange = query.value(7).toString();
        auto user = query.value(8).toString();

        auto wallet = new WalletOperation(id, walletID, coin, exchange, user, amount, available, retired, fiat, QDateTime::fromString(date));
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
            auto color = query.value(8).toString();
            auto type = query.value(9).toString();

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

QString SQLManager::dateTimeToUTC0(QDateTime time, QString exchange)
{
    if (exchange == "B2M")
    {
        QTimeZone zone("Europe/Madrid");
        time.setTimeZone(zone);
        std::cout << time.toString(Qt::ISODateWithMs).replace('T', ' ').toStdString() << " ";
        std::cout << time.toUTC().toString(Qt::ISODateWithMs).replace('T', ' ').toStdString() << std::endl;
        return time.toUTC().toString(Qt::ISODateWithMs).replace('T', ' ').replace('Z', "");
    }
    else if(exchange == "Binance" or exchange == "Crypto" or exchange == "Coinbase")
    {
        // No conversion required
        return time.toString(Qt::ISODateWithMs).replace('T', ' ').replace('Z', "");
    }
}

void SQLManager::updateDateTimeUTCFromQTFormat(QString table, QString id, QDateTime time, QString exchange)
{
    QSqlQuery query = QSqlQuery(database);

    std::cout << "File:SQLManager.cpp Function: updateDateTimeUTCFromQTFormat" << std::endl;

    query.prepare("UPDATE WalletOperations"
                  " SET dateTimeUTC=:time"
                  " WHERE id=:id");
    query.bindValue(":id", id);
    query.bindValue(":time", dateTimeToUTC0(time, exchange));
    query.exec();
}



