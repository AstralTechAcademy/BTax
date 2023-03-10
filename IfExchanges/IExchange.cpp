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
    qDebug() << "[" << QString::fromStdString(__METHOD_NAME__.data()) << "] STARTED";

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
            qDebug() << "[IExchange::" << __func__ << "] " << "Coin ID "<< o->getPair2() << " not found";

        if(coinID.isEmpty())
            opsWithoutPrice.push_back(coinID + " " + o->getDateTime().toString()); 
        else
        {
            qDebug() << "Operation: " << coinID  << " " << o->getDateTime();
            auto price = coingecko->getPrice(coinID, o->getDateTime());
            if(price == std::nullopt)
            {
                opsWithoutPrice.push_back(coinID + " " + o->getDateTime().toString()); 
            }
            else
            {
                qDebug() << "  Price: " << price.value();
                o->setPair2AmountFiat(price.value());
            }
        }

    }
    mutex.unlock();

    qDebug() << "[" <<  QString::fromStdString(__METHOD_NAME__.data()) << "] FINISHED";
        
    if(opsWithoutPrice.size() > 0)
    {
        qDebug() << "[IExchange::" << __func__ << "] " << "There are " << opsWithoutPrice.size()  << " operations without fiat price";
        for(auto l : opsWithoutPrice)
            qDebug() << "[IExchange::" << __func__ << "] " << l;
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
