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
    LOG_DEBUG("Importing file %s", qPrintable(csvPath));

    QFile csv;
    csv.setFileName(csvPath);
    if(csv.exists() == false)
    {
        LOG_ERROR("File does not exist %s", qPrintable(csvPath));
        return std::nullopt;
    }
    return parse(csv);
}

bool IExchange::getFiatPrice(QList<std::shared_ptr<Operation>>& operations)
{
    LOG_INFO("getFiatPrice");

    QList<QString> opsWithoutPrice;
    auto market = MarketDataFactory::createMarketData("YFinance");
    mutex.lock();
    for(auto o : operations)
    {
        QString coinID = o->getPair2().toUpper();

        LOG_DEBUG("Operation: %s %s",  qPrintable(coinID), qPrintable(o->getDateTime().toString()));
        auto price = market->getPrice(coinID, o->getDateTime());
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
    mutex.unlock();        
    if(opsWithoutPrice.size() > 0)
    {
        LOG_WARN("There are %d operations without fiat price", opsWithoutPrice.size());
        for(auto l : opsWithoutPrice)
            LOG_DEBUG("%s", qPrintable(l));
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
