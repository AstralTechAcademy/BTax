//
// Created by gabridc on 10/5/22.
//

#ifndef BROKER_YFINANCE_H
#define BROKER_YFINANCE_H
#include "IMarketData.h"
#include <QNetworkRequest>

class YFinance : public IMarketData{

public:

    enum EN_ResponseCode
    {
        UNKNOWN = 0,
        OK,
        RATE_LIMIT = 429
    };
    YFinance(void);
    static std::shared_ptr<YFinance> getInstance(void);
    virtual std::optional<QMap<QString, QString>> getCoins(void);
    virtual std::optional<QString> getCoinID(const QString& coinName);
    virtual std::optional<double> getCurrentPrice(const QString& coin);
    virtual std::optional<std::unordered_map<QString, double>> getCurrentPrices(void);
    std::optional<QString> getCoinID(const QString& exchange, const QString& coin);
    std::optional<double> getPrice(const QString& coin, const QDateTime& date);
    virtual uint32_t processResponse(QJsonDocument& doc);


private:
    static std::shared_ptr<YFinance> instance_;

};


#endif //BROKER_YFINANCE_H
