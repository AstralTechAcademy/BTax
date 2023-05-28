//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_BINANCE_H
#define BROKER_BINANCE_H
#include "IExchange.h"

class Binance : public IExchange {

public:
    Binance(void);
    virtual bool parseHeader(QFile& csv);
    virtual bool parseBody(QFile& csv);
    virtual QDateTime datetimeStrToDatetime(QByteArray dtimeStr);
private:
    char separator_ = ',';
};


#endif //BROKER_BINANCE_H
