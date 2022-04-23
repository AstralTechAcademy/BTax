//
// Created by gabridc on 19/4/22.
//

#include "ExchangeFactory.h"
#include "Cryptocom.h"
#include "B2m.h"


std::shared_ptr<IExchange> ExchangeFactory::createExchange(const QString& exchange) noexcept
{
    if("Crypto")
        return std::make_shared<Cryptocom>();
    else if("B2m")
        return std::make_shared<B2m>();

    return nullptr;
}