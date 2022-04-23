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

class IExchange {
public:
    virtual std::optional<QList<std::shared_ptr<Operation>>> import(const QString& csvPath) = 0;
    virtual QList<std::shared_ptr<Operation>> parse(QFile& csv) = 0;
};


#endif //BROKER_IEXCHANGE_H
