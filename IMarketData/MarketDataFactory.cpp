//
// Created by gabridc on 19/4/22.
//

#include "MarketDataFactory.h"
#include "Coingecko.h"


std::shared_ptr<IMarketData> MarketDataFactory::createMarketData(const QString& market) noexcept
{
    std::cout << market.toStdString() << std::endl;
    if("Coingecko" == market)
        return std::make_shared<Coingecko>();
    return nullptr;
}