//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_B2MV2_H
#define BROKER_B2MV2_H
#include "IExchange.h"

class B2mV2 : public IExchange {

public:
    B2mV2(void);
    virtual bool parseHeader(QFile& csv);
    virtual bool parseBody(QFile& csv);
    virtual QDateTime datetimeStrToDatetime(QByteArray dtimeStr);
private:
    char separator_ = ',';
};


#endif //BROKER_B2MV2_H
