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

class IExchange {
public:

    virtual std::optional<QList<std::shared_ptr<Operation>>> parse(QFile& csv) = 0;
    virtual QDateTime datetimeStrToDatetime(QByteArray dtimeStr) = 0;
    virtual std::optional<QList<std::shared_ptr<Operation>>> read(const QString& csvPath)
    {
        qDebug() << "File: IExchange.h Func: Import Description: Importing file " << csvPath;
        QFile csv;
        csv.setFileName(csvPath);
        if(csv.exists() == false)
        {
            qDebug() << "File: IExchange.h Func: Import Description: File does not exist " << csvPath;
            return std::nullopt;
        }
        return parse(csv);
    }
protected:
    QList<std::shared_ptr<Operation>> operations_;
};


#endif //BROKER_IEXCHANGE_H
