//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_B2M_H
#define BROKER_B2M_H
#include "IExchange.h"

class B2m : public IExchange {

public:
    B2m(void);
    virtual std::optional<QList<std::shared_ptr<Operation>>> import(const QString& csvPath);
    virtual QList<std::shared_ptr<Operation>> parse(QFile& csv);
};


#endif //BROKER_CRYPTOCOM_H
