//
// Created by gabridc on 20/4/22.
//

#include "Importer.h"
#include "SQLManager.h"
#include "IMarketData/MarketDataFactory.h"


Importer::Importer(std::shared_ptr<BrokerManager> brokerManager, const QObject *o)
{
    brokerManager_ = brokerManager;
};

bool Importer::import(const QString& exchange, const QString& csvPath) noexcept
{
    auto exch = ExchangeFactory::createExchange(exchange);
    auto path = csvPath;
    if(csvPath.contains("file://"))
        path = csvPath.split("file://")[1];
    operations_ = exch->import(path);

    if(operations_ == std::nullopt)
        return false;

    qDebug() << "File: Importer Func: import Description: Operations imported " << operations_->size();
    auto coingecko = MarketDataFactory::createMarketData("Coingecko");
    auto ops = operations_.value();
    for(auto index = 0; index < ops.size();)
    {
        if (!brokerManager_->isDuplicated(ops[index]))
        {

            if(ops[index]->getPair2AmountFiat() == 0.0)
            {
                auto coinID = coingecko->getCoinID("Binance", ops[index]->getPair2());
                if(coinID != std::nullopt)
                {
                    auto price = coingecko->getPrice(coinID.value(), QDateTime::fromString(ops[index]->getDate()));

                    if(price == std::nullopt)
                        ops[index]->setPair2AmountFiat(-1.0);
                    else
                    {
                        ops[index]->setPair2AmountFiat(price.value());
                        qDebug() << "Price: " << ops[index]->getPair2AmountFiat();
                    }

               }
            }
            if(ops[index]->getPair2AmountFiat() != -1.0)
            {
                auto res = brokerManager_->newOperation(exchange, ops[index]);
                switch(res)
                {
                    case BrokerManager::NewOperationRes::ADDED:
                        qDebug() << "File: Importer Func: import Description: Operación registrada";
                        opsAdded_.push_back(ops[index]);
                        index++;
                        break;
                    case BrokerManager::NewOperationRes::ORI_WALLET_NOT_EXIST:
                        qDebug() << "File: Importer Func: import Description: Wallet origen no existe";
                        opsWithError_.push_back(ops[index]);
                        index++;
                        break;
                    case BrokerManager::NewOperationRes::NOT_ADDED:
                        qDebug() << "File: Importer Func: import Description: Error desconocido";
                        opsWithError_.push_back(ops[index]);
                        index++;
                        break;
                    default:
                        qDebug() << "File: Importer Func: import Description: Error desconocido";
                        opsWithError_.push_back(ops[index]);
                        index++;
                        break;
                }
            }
            else
            {
                qDebug() << "File: Importer Func: import Description: No se ha obtenido el precio desde coingecko. Try again ";
            }
        }
        else // IsDuplicated
        {
            qDebug() << "File: Importer Func: import Description: Operación duplicada";
            opsAlrdyAdded_.push_back(ops[index]);
            index++;
        }
    }

    // Return TRUE todas las transacciones han sido añadidas o ya estaban en la BD
    return opsWithError_.empty() == false ? true : false;
}