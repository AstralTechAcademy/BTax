//
// Created by gabridc on 19/4/22.
//

#include "ExchangeFactory.h"
#include "Cryptocom.h"
#include "B2m.h"
#include "Binance.h"


std::shared_ptr<IExchange> ExchangeFactory::createExchange(const QString& exchange) noexcept
{
    std::cout << exchange.toStdString() << std::endl;
    if("Crypto" == exchange)
        return std::make_shared<Cryptocom>();
    else if("B2M" == exchange)
        return std::make_shared<B2m>();
    else if("Binance" == exchange)
        return std::make_shared<Binance>();
    return nullptr;
}

std::shared_ptr<IExchange> ExchangeFactory::createExchange(const EN_Exchange exchange) noexcept
{
    std::cout << exchange << std::endl;
    if(EN_Exchange::CRYPTO == exchange)
        return std::make_shared<Cryptocom>();
    else if(EN_Exchange::B2M == exchange)
        return std::make_shared<B2m>();
    else if(EN_Exchange::BINANCE == exchange)
        return std::make_shared<Binance>();
    return nullptr;
}