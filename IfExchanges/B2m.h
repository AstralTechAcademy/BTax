//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_B2M_H
#define BROKER_B2M_H
#include "IExchange.h"

class B2m : public IExchange {

public:
    B2m(void);
    virtual bool parseHeader(QFile& csv);
    virtual bool parseBody(QFile& csv);
    virtual QDateTime datetimeStrToDatetime(QByteArray dtimeStr);
private:
    char separator_ = ',';
};


#endif //BROKER_B2M_H
