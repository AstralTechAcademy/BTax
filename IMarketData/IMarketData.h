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
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

class IMarketData {
public:
    virtual std::optional<double> getCurrentPrice(const QString& fiat) = 0;
    virtual std::optional<std::unordered_map<QString, double>> getCurrentPrices(void) = 0;

    virtual QJsonDocument send(std::shared_ptr<QNetworkRequest> request)
    {
        QNetworkAccessManager nManager;

        auto reply = nManager.get(*request.get());
        while (!reply->isFinished()) qApp->processEvents();
        QByteArray response_data = reply->readAll();

        //std::cout << response_data.toStdString() << std::endl;
        return QJsonDocument().fromJson(response_data);;
    };

};




#endif //BROKER_IEXCHANGE_H