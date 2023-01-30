//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_IEXCHANGE_H
#define BROKER_IEXCHANGE_H
#include <memory>
#include <QList>
#include <QFile>
#include <QDateTime>
#include <QMap>
#include "Operation.h"
#include "BrokerManager.h"
#include "MarketDataFactory.h"

class IExchange {
public:

    enum class EN_COLUMN_NAMES
    {
        DATE = 0,
        TYPE,
        PAIR1,
        PAIR1_AMOUNT,
        FEE,
        FEE_AMOUNT,
        PAIR2,
        PAIR2_AMOUNT,
        PAIR2_AMOUNT_FIAT,
        FIAT_CURRENCY
    };
    IExchange(void) =default;

    // Comunes
    virtual std::optional<QList<std::shared_ptr<Operation>>> parse(QFile& csv);
    virtual std::optional<QList<std::shared_ptr<Operation>>> read(const QString& csvPath);
    bool getFiatPrice(QList<std::shared_ptr<Operation>>& operations);
    void calcFiatReward(QList<std::shared_ptr<Operation>>& operations);

    // Virtual puras
    virtual bool parseHeader(QFile& csv) = 0;
    virtual bool parseBody(QFile& csv) = 0;
    virtual QDateTime datetimeStrToDatetime(QByteArray dtimeStr) = 0;
protected:
    QList<std::shared_ptr<Operation>> operations_;
    QMap<EN_COLUMN_NAMES, int> header_;
};


#endif //BROKER_IEXCHANGE_H
