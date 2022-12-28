//
// Created by gabridc on 19/4/22.
//

#include "Binance.h"
#include <QFile>
#include "DBLocal.h"

Binance::Binance(void){};


QDateTime Binance::datetimeStrToDatetime(QByteArray dtimeStr)
{
    return QDateTime::fromString(dtimeStr, Qt::DateFormat::ISODateWithMs); // Date
}

bool Binance::parseHeader(QFile& csv)
{
    QByteArray line = csv.readLine();
    auto columns = line.split(separator_);
    for(auto index = 0; index < columns.size(); index++)
    {
        if(columns[index] == "UTC_Time")
            header_.insert(EN_COLUMN_NAMES::DATE, index);
        if(columns[index] == "Operation")
            header_.insert(EN_COLUMN_NAMES::TYPE, index);

        if(columns[index] == "Coin")
            header_.insert(EN_COLUMN_NAMES::PAIR2, index);
        if(columns[index] == "Change")
            header_.insert(EN_COLUMN_NAMES::PAIR2_AMOUNT, index);

    }

    return header_.size() == 4;
}

bool Binance::parseBody(QFile& csv)
{
    while (!csv.atEnd()) {
        QByteArray line = csv.readLine();
        if(line.isEmpty() == false and line.size() > 1 and line.contains(separator_))
        {
            auto lineV = line.split(separator_);
            //Defaults
            double pair1Amount = 0.0;
            double pair1AmountFiat = 1.0;
            double pair2AmountFiat = 0.0;

            if(lineV.size() > 0 && lineV[header_[EN_COLUMN_NAMES::TYPE]] == "POS savings interest")
            {
                qDebug() << "Operation: " << lineV[header_[EN_COLUMN_NAMES::PAIR2]] << " " << lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble();
                operations_.push_back(std::make_shared<Operation>(0, BrokerManager::DEF_FIAT,  // Pair 1 Coin
                                                                    lineV[header_[EN_COLUMN_NAMES::PAIR2]],  // Pair 2 Coin
                                                                    pair1Amount, pair1AmountFiat,
                                                                    lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble(),
                                                                    pair2AmountFiat,
                                                                    0.0, 1.0, "Accepted",
                                                                    cnvDateTime2StrFormat(datetimeStrToDatetime(lineV[header_[EN_COLUMN_NAMES::DATE]]), EN_DateFormat::DMYhms),                                                                 "", "Earn", 
                                                                    pair2AmountFiat*lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble()) // Ganancia
                                                                    );
            }
        }
    }

    auto allOpsWithPrice = getFiatPrice(operations_);
    if(operations_.size() > 0 && allOpsWithPrice)
        return true;
    else
        return false;
}