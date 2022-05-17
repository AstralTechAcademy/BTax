//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_EXCHANGEFACTORY_H
#define BROKER_EXCHANGEFACTORY_H
#include "IExchange.h"
#include <memory>
class ExchangeFactory {

public:
    static std::shared_ptr<IExchange> createExchange(const QString& exchange) noexcept;

};


#endif //BROKER_EXCHANGEFACTORY_H
