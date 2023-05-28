//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_CRYPTOCOM_H
#define BROKER_CRYPTOCOM_H
#include "IExchange.h"

class Cryptocom : public IExchange {

public:
    Cryptocom(void);
    virtual bool parseHeader(QFile& csv);
    virtual bool parseBody(QFile& csv);
    virtual QDateTime datetimeStrToDatetime(QByteArray dtimeStr);

private:
    char separator_ = ',';
};


#endif //BROKER_CRYPTOCOM_H
