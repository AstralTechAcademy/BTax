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
    auto url = QUrl("http://" + Config::getInstance()->getYfinServer() + ":" + QString::number(Config::getInstance()->getYfinPort()) +
                            "/yfinance/getPrice?ticker=" + coin.toUpper()+ "-EUR" + 
                            "&datetime=" + QString::number(date.date().year()) + "-" + QString::number(date.date().month()) + "-" + QString::number(date.date().day()) + " 01:00:00");
    
    LOG_INFO("%s", qPrintable(url.toString()));
    request->setUrl(url);
    auto response_doc = IMarketData::send(request);
    auto resCode = processResponse(response_doc);
    if(resCode != EN_ResponseCode::OK)
        return std::nullopt;
    else
        LOG_INFO("%d %s", resCode, qPrintable(response_doc.toJson()));


    if(response_doc.isNull() == false)
    {
        // TODO
        qDebug() << response_doc;
        return std::nullopt;

    }
    else
    {
        qDebug() << "1";
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