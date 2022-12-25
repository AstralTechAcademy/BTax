//
// Created by gabridc on 19/4/22.
//

#include "Binance.h"
#include <QFile>
#include "DBLocal.h"

Binance::Binance(void){};


std::optional<QList<std::shared_ptr<Operation>>> Binance::parse(QFile& csv)
{
    csv.open(QIODevice::ReadOnly);
    uint8_t firstLine = 0;
    QList<std::shared_ptr<Operation>> operations;
    QMap<EN_COLUMN_NAMES, int> header;

    while (!csv.atEnd()) {
        QByteArray line = csv.readLine();
        if(firstLine == 0) // HEADER
        {
            auto columns = line.split(',');
            for(auto index = 0; index < columns.size(); index++)
            {

                if(columns[index] == "UTC_Time")
                    header.insert(EN_COLUMN_NAMES::DATE, index);
                if(columns[index] == "Operation")
                    header.insert(EN_COLUMN_NAMES::TYPE, index);

                if(columns[index] == "Coin")
                    header.insert(EN_COLUMN_NAMES::PAIR2, index);
                if(columns[index] == "Change")
                    header.insert(EN_COLUMN_NAMES::PAIR2_AMOUNT, index);

            }
        }
        else
        {
            auto lineV = line.split(',');
            //Defaults
            double pair1Amount = 0.0;
            double pair1AmountFiat = 1.0;
            double pair2AmountFiat = 0.0;

            if(lineV[header[EN_COLUMN_NAMES::TYPE]] == "POS savings interest")
            {
                qDebug() << "Operation: " << lineV[header[EN_COLUMN_NAMES::PAIR2]] << " " << lineV[header[EN_COLUMN_NAMES::PAIR2_AMOUNT_FIAT]].toDouble();
                operations.push_back(std::make_shared<Operation>(0, BrokerManager::DEF_FIAT,  // Pair 1 Coin
                                                                 lineV[header[EN_COLUMN_NAMES::PAIR2]],  // Pair 2 Coin
                                                                 pair1Amount, pair1AmountFiat,
                                                                 lineV[header[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble(),
                                                                 pair2AmountFiat,
                                                                 0.0, 1.0, "Accepted",
                                                                 cnvDateTime2StrFormat(datetimeStrToDatetime(lineV[header[EN_COLUMN_NAMES::DATE]]), EN_DateFormat::DMYhms),                                                                 "", "Earn", 
                                                                 pair2AmountFiat*lineV[header[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble()) // Ganancia
                                                                 );
            }
        }

        firstLine = 1;
    }

    getFiatPrice(operations);

    if(operations.size() > 0)
        return operations;
    else
        return std::nullopt;
}

QDateTime Binance::datetimeStrToDatetime(QByteArray dtimeStr)
{
    return QDateTime::fromString(dtimeStr, Qt::DateFormat::ISODateWithMs); // Date
}

bool Binance::getFiatPrice(QList<std::shared_ptr<Operation>>& operations)
{
    Coingecko coingecko;

    for(auto o : operations)
    {
        auto id = coingecko.getCoinID(o->getPair2());
        if(id == std::nullopt)
            return false;
        
        qDebug() << o->getDate() << " " << o->getDateTime();

        auto price = coingecko.getPrice(id.value(), o->getDateTime());

        if(price == std::nullopt)
            return false;
        
        qDebug() << "Price: " << id.value()  << " " << o->getDateTime() << " " << price.value();

    }
        

    return false;
}