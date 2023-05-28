//
// Created by gabridc on 19/4/22.
//

#include "Cryptocom.h"

Cryptocom::Cryptocom(void){};

QDateTime Cryptocom::datetimeStrToDatetime(QByteArray dtimeStr)
{
    return QDateTime::fromString(dtimeStr, Qt::DateFormat::ISODate); // Date
}

bool Cryptocom::parseHeader(QFile& csv)
{
    QByteArray line = csv.readLine();

    auto columns = line.split(separator_);
    for(auto index = 0; index < columns.size(); index++)
    {

        if(columns[index] == "Timestamp (UTC)")
            header_.insert(EN_COLUMN_NAMES::DATE, index);
        if(columns[index] == "Transaction Description")
            header_.insert(EN_COLUMN_NAMES::TYPE, index);

        if(columns[index] == "Currency")
            header_.insert(EN_COLUMN_NAMES::PAIR1, index);
        if(columns[index] == "Amount")
            header_.insert(EN_COLUMN_NAMES::PAIR1_AMOUNT, index);


        if(columns[index] == "To Currency")
            header_.insert(EN_COLUMN_NAMES::PAIR2, index);
        if(columns[index] == "To Amount")
            header_.insert(EN_COLUMN_NAMES::PAIR2_AMOUNT, index);

        if(columns[index] == "Native Currency")
            header_.insert(EN_COLUMN_NAMES::FIAT_CURRENCY, index);
        if(columns[index] == "Native Amount")
            header_.insert(EN_COLUMN_NAMES::PAIR2_AMOUNT_FIAT, index);
    }
        
}

bool Cryptocom::parseBody(QFile& csv)
{
    QByteArray line = csv.readLine();

    auto columns = line.split(separator_);
    while (!csv.atEnd()) {
            auto lineV = line.split(',');
            if(lineV[header_[EN_COLUMN_NAMES::TYPE]] == "Card Cashback")
            {
                operations_.push_back(std::make_shared<Operation>(0, lineV[header_[EN_COLUMN_NAMES::FIAT_CURRENCY]], lineV[header_[EN_COLUMN_NAMES::PAIR1]], 0.0, 1.0,
                                                                 lineV[header_[EN_COLUMN_NAMES::PAIR1_AMOUNT]].toDouble(),
                                                                 lineV[header_[EN_COLUMN_NAMES::PAIR1_AMOUNT]].toDouble() / lineV[header_[EN_COLUMN_NAMES::PAIR1_AMOUNT]].toDouble(),
                                                                 0.0, 1.0, "Accepted",
                                                                 QDateTime::fromString(lineV[header_[EN_COLUMN_NAMES::DATE]], Qt::DateFormat::ISODate).toString(),
                                                                 "", "Cashback", lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT_FIAT]].toDouble()));
            }        
    }
}