//
// Created by gabridc on 20/4/22.
//

#include "Importer.h"
#include "SQLManager.h"


Importer::Importer(std::shared_ptr<BrokerManager> brokerManager, const QObject *o)
{
    brokerManager_ = brokerManager;
};

bool Importer::import(const QString& exchange, const QString& csvPath) const noexcept
{
    auto exch = MarketDataFactory::createExchange(exchange);
    auto operations = exch->import(csvPath);

    if(operations == std::nullopt)
        return false;
    else
    {
        for(auto operation : operations.value())
        {
            brokerManager_->newOperation(exchange, operation);
        }
        return true;
    }
}