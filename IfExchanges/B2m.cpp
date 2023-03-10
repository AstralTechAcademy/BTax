//
// Created by gabridc on 19/4/22.
//

#include "B2m.h"
#include <QFile>

B2m::B2m(void){};

QDateTime B2m::datetimeStrToDatetime(QByteArray dtimeStr)
{
    return QDateTime::fromString(dtimeStr, Qt::DateFormat::ISODateWithMs); // Date
}

bool B2m::parseHeader(QFile& csv)
{
    QByteArray line = csv.readLine();
    line = csv.readLine();
    auto columns = line.split(separator_);
    for(auto index = 0; index < columns.size(); index++)
    {
        if(columns[index] == "DATE")
            header_.insert(EN_COLUMN_NAMES::DATE, index);
        if(columns[index] == "METHOD")
            header_.insert(EN_COLUMN_NAMES::TYPE, index);

        if(columns[index] == "FROM CURRENCY") // Se coge usando la columna de fees. Moneda por defecto
            header_.insert(EN_COLUMN_NAMES::PAIR1, index);
        if(columns[index] == "FROM AMOUNT") // Se coge un valor 0 porque siempre es 0 el valor en EUR al ser EARN
            header_.insert(EN_COLUMN_NAMES::PAIR1_AMOUNT, index);

        if(columns[index] == "FEE CURRENCY") // Se coge usando la columna de fees. Moneda por defecto
            header_.insert(EN_COLUMN_NAMES::FEE, index);
        if(columns[index] == "FEE AMOUNT") // Se coge un valor 0 porque siempre es 0 el valor en EUR al ser EARN
            header_.insert(EN_COLUMN_NAMES::FEE_AMOUNT, index);

        if(columns[index] == "TO CURRENCY")
            header_.insert(EN_COLUMN_NAMES::PAIR2, index);
        if(columns[index] == "TO AMOUNT")
            header_.insert(EN_COLUMN_NAMES::PAIR2_AMOUNT, index);

        if(columns[index] == "FEE CURRENCY")
            header_.insert(EN_COLUMN_NAMES::FIAT_CURRENCY, index);
        if(columns[index] == "TO RATE EUR")
            header_.insert(EN_COLUMN_NAMES::PAIR2_AMOUNT_FIAT, index);
    }

    return header_.size() == 10;
}

bool B2m::parseBody(QFile& csv)
{
    while (!csv.atEnd()) {
        QByteArray line = csv.readLine();
        if(line.isEmpty() == false and line.size() > 1 and line.contains(separator_))
        {
            auto lineV = line.split(separator_);
            if(lineV.size() > 0 && lineV[header_[EN_COLUMN_NAMES::TYPE]] == "earn")
            {
                //qDebug() << "Operation: " << lineV[header_[EN_COLUMN_NAMES::PAIR1]] << " " << lineV[header_[EN_COLUMN_NAMES::PAIR2]] << " " << lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT_FIAT]].toDouble();
                operations_.push_back(std::make_shared<Operation>(0, 
                            lineV[header_[EN_COLUMN_NAMES::FEE]],  // Pair 1 Coin
                            lineV[header_[EN_COLUMN_NAMES::PAIR2]], // Pair 2 Coin
                            0.0, // Pair1Amount
                            1.0, // Pair1AmountFiat
                            lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble(), // Pair2Amount
                            lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT_FIAT]].toDouble(), // Pair2AmountFiat
                            0.0, // Fees Amount
                            1.0, // Fees Amount fiat
                            "Accepted", // Status
                            cnvDateTime2StrFormat(datetimeStrToDatetime(lineV[header_[EN_COLUMN_NAMES::DATE]]), EN_DateFormat::DMYhms), // Date //TODO: This variable must be dateTime type
                            "", // Description 
                            "Earn", // Type
                            lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT_FIAT]].toDouble()*lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble() // Ganancia
                            ));
            }
        }
    }

    if(operations_.size() > 0)
        return true;
    else
        return false;

}