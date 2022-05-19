//
// Created by gabridc on 10/5/22.
//

#ifndef BROKER_COINGECKO_H
#define BROKER_COINGECKO_H
#include "IMarketData.h"
#include <QNetworkRequest>

class Coingecko : public IMarketData{

public:
    virtual std::optional<double> getCurrentPrice(const QString& coin);
    virtual std::optional<std::unordered_map<QString, double>> getCurrentPrices(void);
    std::optional<QString> getCoinID(const QString& exchange, const QString& coin);

};


#endif //BROKER_COINGECKO_H
