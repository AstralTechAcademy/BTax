//
// Created by gabridc on 19/4/22.
//

#ifndef BROKER_CRYPTOCOM_H
#define BROKER_CRYPTOCOM_H
#include "IExchange.h"

class Cryptocom : public IExchange {

public:
    Cryptocom(void);
    virtual std::optional<QList<std::shared_ptr<Operation>>> import(const QString& csvPath);
    virtual QList<std::shared_ptr<Operation>> parse(QFile& csv);
};


#endif //BROKER_CRYPTOCOM_H
