//
// Created by gabridc on 10/5/22.
//

#include "YFinance.h"
#include "BrokerManager.h"
#include "Config.h"
#include <iostream>
#include <QThread>
#include <tuple>

std::shared_ptr<YFinance> YFinance::instance_ = nullptr;

YFinance::YFinance(void)
{
}

std::shared_ptr<YFinance> YFinance::getInstance(void)
{
    if(instance_ == nullptr)
        instance_ = std::make_shared<YFinance>();    
    return instance_;
}

std::optional<double> YFinance::getCurrentPrice(const QString& coin)
{   
    return std::nullopt;
};

std::optional<std::unordered_map<QString, double>> YFinance::getCurrentPrices(void)
{
    return std::nullopt;
};

std::optional<QString> YFinance::getCoinID(const QString& exchange, const QString& coin)
{
    std::shared_ptr<QNetworkRequest> request = std::make_shared<QNetworkRequest>();
    request->setUrl(QUrl("https://api.coingecko.com/api/v3/coins/list"));
    request->setRawHeader("accept", "application/json");
    auto response_doc = IMarketData::send(request);
    auto resCode = processResponse(response_doc);


    if(response_doc.isNull() == false && response_doc.isObject() && resCode != EN_ResponseCode::OK)
        return std::nullopt;

    if(response_doc.isNull() == false && response_doc.isArray() == true)
    {
         for (auto c : response_doc.array())
        {
            if (c.toObject().value("symbol").toString() == coin.toLower())
                return c.toObject().value("id").toString();
        }
         return std::nullopt;
    }
    else
        return std::nullopt;
}

std::optional<double> YFinance::getPrice(const QString& coin, const QDateTime& date)
{

    std::shared_ptr<QNetworkRequest> request = std::make_shared<QNetworkRequest>();
    auto dateStr = QString::number(date.date().year()) + "-" + QString::number(date.date().month()) + "-" + QString::number(date.date().day());
    auto url = QUrl("http://" + Config::getInstance()->getYfinServer() + ":" + QString::number(Config::getInstance()->getYfinPort()) +
                            "/yfinance/getPrice?ticker=" + coin.toUpper() + 
                            "&datetime=" + dateStr + " 01:00:00");
    
    request->setUrl(url);
    auto response_doc = IMarketData::send(request);
    auto resCode = processResponse(response_doc);
    if(resCode != EN_ResponseCode::OK)
        return std::nullopt;

    if(response_doc.isNull() == false)
    {
        if(response_doc.isObject())
        {
            auto keys = response_doc.object().keys();

            if(keys.contains("error"))
                return std::nullopt;

            if(keys.contains("datetime") and keys.contains("ticker") and keys.contains("price"))
            {
                LOG_DEBUG("Ticker: %s Date: %s Price: %lf", qPrintable(coin.toUpper()), qPrintable(dateStr), response_doc.object()["price"].toDouble());
                return response_doc.object()["price"].toDouble();
            }    
                
        }
    }
    else
    {
        return std::nullopt;
    }
       
}

std::optional<QMap<QString, QString>> YFinance::getCoins(void)
{
    return std::nullopt;
}

std::optional<QString> YFinance::getCoinID(const QString& coinName)
{        
    return std::nullopt;
}

uint32_t YFinance::processResponse(QJsonDocument& doc)
{
    //TODO
    return EN_ResponseCode::OK;
}