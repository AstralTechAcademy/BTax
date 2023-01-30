//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_MARKETDATAFACTORY_H
#define BROKER_MARKETDATAFACTORY_H
#include "IMarketData.h"
#include <memory>

class MarketDataFactory {

public:
    static std::shared_ptr<IMarketData> createMarketData(const QString& market) noexcept
    {
        std::cout << market.toStdString() << std::endl;
        if("Coingecko" == market)
            return Coingecko::getInstace();
        return nullptr;
    }

};

#endif //BROKER_MARKETDATAFACTORY_H
