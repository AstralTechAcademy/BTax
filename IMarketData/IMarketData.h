//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_IMARKETDATA_H
#define BROKER_IMARKETDATA_H
#include <memory>
#include <qcoreapplication.h>
#include <QList>
#include <QFile>
#include <QDateTime>
#include <QMap>
#include <QNetworkRequest>
#include <unordered_map>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include <QThread>

class IMarketData {
public:
    virtual std::optional<QMap<QString, QString>> getCoins(void) = 0;
    virtual std::optional<double> getCurrentPrice(const QString& fiat) = 0;
    virtual std::optional<QString> getCoinID(const QString& coinName) = 0;
    virtual std::optional<std::unordered_map<QString, double>> getCurrentPrices(void) = 0;
    virtual std::optional<double> getPrice(const QString& coin, const QDateTime& date) = 0;
    virtual std::optional<QString> getCoinID(const QString& exchange, const QString& coin) = 0;
    virtual uint32_t processResponse(QJsonDocument& doc) = 0;
    virtual QJsonDocument send(std::shared_ptr<QNetworkRequest> request)
    {
        QThread::msleep(10000);
        QNetworkAccessManager nManager;

        auto reply = nManager.get(*request.get());
        while (!reply->isFinished()) qApp->processEvents();
        QByteArray response_data = reply->readAll();
        return QJsonDocument().fromJson(response_data);
    };

};




#endif //BROKER_IEXCHANGE_H
