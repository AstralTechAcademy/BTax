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

    virtual std::optional<QList<std::shared_ptr<Operation>>> parse(QFile& csv) = 0;
    virtual void parseHeader(QFile& csv) = 0;
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
    QMap<EN_COLUMN_NAMES, int> header_;
};


#endif //BROKER_IEXCHANGE_H
