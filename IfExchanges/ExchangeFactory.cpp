//
// Created by gabridc on 19/4/22.
//

#include "ExchangeFactory.h"
#include <QDebug>
#include "Cryptocom.h"
#include "B2m.h"
#include "B2mV2.h"
#include "Binance.h"


std::shared_ptr<IExchange> ExchangeFactory::createExchange(const QString& exchange) noexcept
{
    if("Crypto" == exchange)
        return std::make_shared<Cryptocom>();
    else if("B2M" == exchange)
        return std::make_shared<B2m>();
    else if("B2MV2" == exchange)
        return std::make_shared<B2mV2>();        
    else if("Binance" == exchange)
        return std::make_shared<Binance>();
    return nullptr;
}

std::shared_ptr<IExchange> ExchangeFactory::createExchange(const EN_Exchange exchange) noexcept
{
    if(EN_Exchange::CRYPTO == exchange)
    {
        qDebug() << "[INFO][ExchangeFactory::createExchange] Exchange created is CRYPTO";
        return std::make_shared<Cryptocom>();
    }
    else if(EN_Exchange::B2M == exchange)
    {
        qDebug() << "[INFO][ExchangeFactory::createExchange] Exchange created is B2M";
        return std::make_shared<B2m>();
    }
    else if(EN_Exchange::B2M_V2 == exchange)
    {
        qDebug() << "[INFO][ExchangeFactory::createExchange] Exchange created is B2M V2";
        return std::make_shared<B2mV2>();   
    }
    else if(EN_Exchange::BINANCE == exchange)
    {
        qDebug() << "[INFO][ExchangeFactory::createExchange] Exchange created is BINANCE";
        return std::make_shared<Binance>();
    }

    return nullptr;
}