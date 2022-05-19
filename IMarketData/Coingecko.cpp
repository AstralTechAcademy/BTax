//
// Created by gabridc on 10/5/22.
//

#include "Coingecko.h"
#include "BrokerManager.h"
#include <iostream>
#include <tuple>

std::optional<double> Coingecko::getCurrentPrice(const QString& coin)
{
    std::shared_ptr<QNetworkRequest> request = std::make_shared<QNetworkRequest>();
    request->setUrl(QUrl("https://api.coingecko.com/api/v3/coins/" + coin.toLower() + "?localization=false&tickers=false&market_data=true&community_data=false&developer_data=false&sparkline=false"));
    request->setRawHeader("accept", "application/json");
    auto response_doc = IMarketData::send(request);

    if(response_doc.isNull() == false && response_doc.isObject() == true)
        return response_doc.object().value("market_data").toObject().value("current_price").toObject().value(BrokerManager::DEF_FIAT.toLower()).toDouble();
    else
        return std::nullopt;
};

std::optional<std::unordered_map<QString, double>> Coingecko::getCurrentPrices(void)
{
    std::shared_ptr<QNetworkRequest> request = std::make_shared<QNetworkRequest>();
    request->setUrl(QUrl("https://api.coingecko.com/api/v3/coins/markets?vs_currency=" + BrokerManager::DEF_FIAT.toLower()));
    request->setRawHeader("accept", "application/json");
    auto response_doc = IMarketData::send(request);

    if(response_doc.isNull() == false && response_doc.isArray() == true)
    {
        std::unordered_map<QString, double> result;
        auto data = response_doc.array();

        for (auto i = 0; i < data.size(); i++)
            result.insert({data[i].toObject().value("symbol").toString(), data[i].toObject().value("current_price").toDouble()});
        return result;
    }
    else
        return std::nullopt;
};

std::optional<QString> Coingecko::getCoinID(const QString& exchange, const QString& coin)
{
    std::shared_ptr<QNetworkRequest> request = std::make_shared<QNetworkRequest>();
    request->setUrl(QUrl("https://api.coingecko.com/api/v3/coins/list"));
    request->setRawHeader("accept", "application/json");
    auto response_doc = IMarketData::send(request);

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