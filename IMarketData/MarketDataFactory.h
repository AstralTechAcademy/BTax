//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_MARKETDATAFACTORY_H
#define BROKER_MARKETDATAFACTORY_H
#include "IMarketData.h"
#include <memory>

class MarketDataFactory {

public:
    static std::shared_ptr<IMarketData> createMarketData(const QString& market) noexcept;

};

#endif //BROKER_MARKETDATAFACTORY_H
