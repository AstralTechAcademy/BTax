//
// Created by gabridc on 10/5/22.
//

#ifndef BROKER_COINGECKO_H
#define BROKER_COINGECKO_H
#include "IMarketData.h"
#include <QNetworkRequest>

class Coingecko : public IMarketData{

public:
    Coingecko(void);
    virtual std::optional<QMap<QString, QString>> getCoins(void);
    virtual std::optional<QString> getCoinID(const QString& coinName);
    virtual std::optional<double> getCurrentPrice(const QString& coin);
    virtual std::optional<std::unordered_map<QString, double>> getCurrentPrices(void);
    std::optional<QString> getCoinID(const QString& exchange, const QString& coin);
    std::optional<double> getPrice(const QString& coin, const QDateTime& date);

private:
    QMap<QString, QString> coins_;

};


#endif //BROKER_COINGECKO_H
