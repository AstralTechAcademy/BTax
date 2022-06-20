//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_MARKETDATAFACTORY_H
#define BROKER_MARKETDATAFACTORY_H
#include "IExchange.h"
#include <memory>
class MarketDataFactory {

public:
    static std::shared_ptr<IExchange> createExchange(const QString& exchange) noexcept;

};


#endif //BROKER_MARKETDATAFACTORY_H