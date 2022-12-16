//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_BINANCE_H
#define BROKER_BINANCE_H
#include "IExchange.h"

class Binance : public IExchange {

public:
    Binance(void);
    virtual std::optional<QList<std::shared_ptr<Operation>>> parse(QFile& csv);
    virtual QDateTime datetimeStrToDatetime(QByteArray dtimeStr);
};


#endif //BROKER_BINANCE_H
