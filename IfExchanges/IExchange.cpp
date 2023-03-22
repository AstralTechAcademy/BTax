#include "IExchange.h"
#include "Logger.h"

std::optional<QList<std::shared_ptr<Operation>>> IExchange::parse(QFile& csv)
{
    csv.open(QIODevice::ReadOnly);
    bool res = false;
    res = parseHeader(csv);
    res = parseBody(csv);
    
    if(operations_.size() > 0 && res)
        return operations_;
    else
        return std::nullopt;

}
std::optional<QList<std::shared_ptr<Operation>>> IExchange::read(const QString& csvPath)
{
    LOG_INFO("read");
    LOG_DEBUG("Importing file %s", csvPath);

    QFile csv;
    csv.setFileName(csvPath);
    if(csv.exists() == false)
    {
        LOG_ERROR("File does not exist %s", csvPath);
        return std::nullopt;
    }
    return parse(csv);
}

bool IExchange::getFiatPrice(QList<std::shared_ptr<Operation>>& operations)
{
    LOG_INFO("getFiatPrice");

    QList<QString> opsWithoutPrice;
    auto coingecko = MarketDataFactory::createMarketData("Coingecko");
    auto opt = coingecko->getCoins();
    if(opt == std::nullopt)
        return false;
    QMap<QString, QString> coins = opt.value();
    mutex.lock();
    for(auto o : operations)
    {
        QString coinID;
        if(coins.contains(o->getPair2().toLower()))
            coinID = coins.value(o->getPair2().toLower());
        else
            LOG_ERROR("Coin ID %s not found", qPrintable(o->getPair2()));

        if(coinID.isEmpty())
            opsWithoutPrice.push_back(coinID + " " + o->getDateTime().toString()); 
        else
        {
            LOG_DEBUG("Operation: %s %s",  qPrintable(coinID), qPrintable(o->getDateTime().toString()));
            auto price = coingecko->getPrice(coinID, o->getDateTime());
            if(price == std::nullopt)
            {
                opsWithoutPrice.push_back(coinID + " " + o->getDateTime().toString()); 
            }
            else
            {
                LOG_DEBUG("  Price: %lf", price.value());
                o->setPair2AmountFiat(price.value());
            }
        }

    }
    mutex.unlock();        
    if(opsWithoutPrice.size() > 0)
    {
        LOG_WARN("There are %d operations without fiat price", opsWithoutPrice.size());
        for(auto l : opsWithoutPrice)
            LOG_DEBUG("%s", l.toStdString());
        return false;
    }
    else
        return true;
}

void IExchange::calcFiatReward(QList<std::shared_ptr<Operation>>& operations)
{
    for(auto o : operations)
        o->setGanancia(o->getPair2Amount() * o->getPair2AmountFiat());
}
