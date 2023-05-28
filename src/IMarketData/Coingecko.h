//
// Created by gabridc on 10/5/22.
//

#ifndef BROKER_COINGECKO_H
#define BROKER_COINGECKO_H
#include "IMarketData.h"
#include <QNetworkRequest>

class Coingecko : public IMarketData{

public:

    enum EN_ResponseCode
    {
        UNKNOWN = 0,
        OK,
        RATE_LIMIT = 429
    };
    Coingecko(void);
    static std::shared_ptr<Coingecko> getInstance(void);
    virtual std::optional<QMap<QString, QString>> getCoins(void);
    virtual std::optional<QString> getCoinID(const QString& coinName);
    virtual std::optional<double> getCurrentPrice(const QString& coin);
    virtual std::optional<std::unordered_map<QString, double>> getCurrentPrices(void);
    std::optional<QString> getCoinID(const QString& exchange, const QString& coin);
    std::optional<double> getPrice(const QString& coin, const QDateTime& date);
    virtual uint32_t processResponse(QJsonDocument& doc);


private:

    QMap<QString, QString> coins_;
    static std::shared_ptr<Coingecko> instance_;

};


#endif //BROKER_COINGECKO_H
