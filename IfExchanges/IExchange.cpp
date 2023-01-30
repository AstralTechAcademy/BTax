#include "IExchange.h"


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
    qDebug() << "File: IExchange.h Func: Import Description: Importing file " << csvPath;
    QFile csv;
    csv.setFileName(csvPath);
    if(csv.exists() == false)
    {
        qDebug() << "File: IExchange.h Func: Import Description: File does not exist " << csvPath;
        return std::nullopt;
    }
    return parse(csv);
}

bool IExchange::getFiatPrice(QList<std::shared_ptr<Operation>>& operations)
{
    QList<QString> opsWithoutPrice;
    auto coingecko = MarketDataFactory::createMarketData("Coingecko");
    for(auto o : operations)
    {
        auto id = coingecko->getCoinID(o->getPair2());
        if(id == std::nullopt)
            return false;
        
        auto price = coingecko->getPrice(id.value(), o->getDateTime());
        if(price == std::nullopt)
            opsWithoutPrice.push_back(id.value() + " " + o->getDateTime().toString() );
        
        qDebug() << "Price: " << id.value()  << " " << o->getDateTime() << " " << price.value();
        o->setPair2AmountFiat(price.value());

    }
        
    if(opsWithoutPrice.size() > 0)
    {
        qDebug() << "There are " << opsWithoutPrice.size()  << " operations without fiat price";
        for(auto l : opsWithoutPrice)
            qDebug() << l;
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
