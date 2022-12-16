//
// Created by gabridc on 19/4/22.
//

#include "B2m.h"
#include <QFile>

B2m::B2m(void){};

std::optional<QList<std::shared_ptr<Operation>>> B2m::parse(QFile& csv)
{
    qDebug() << "File: B2m.cpp Func: Parse Description: Parsing CSV file";
    csv.open(QIODevice::ReadOnly);
    uint8_t firstLine = 0;
    operations_.clear();

    QMap<QString, int> header;
    while (!csv.atEnd()) {
        QByteArray line = csv.readLine();
        if(firstLine == 1) // HEADER
        {
            auto columns = line.split(',');
            for(auto index = 0; index < columns.size(); index++)
            {

                if(columns[index] == "DATE")
                    header.insert("DATE", index);
                if(columns[index] == "METHOD")
                    header.insert("TYPE", index);

                if(columns[index] == "FROM CURRENCY") // Se coge usando la columna de fees. Moneda por defecto
                    header.insert("PAIR1", index);
                if(columns[index] == "FROM AMOUNT") // Se coge un valor 0 porque siempre es 0 el valor en EUR al ser EARN
                    header.insert("PAIR1_AMOUNT", index);

                if(columns[index] == "FEE CURRENCY") // Se coge usando la columna de fees. Moneda por defecto
                    header.insert("FEE", index);
                if(columns[index] == "FEE AMOUNT") // Se coge un valor 0 porque siempre es 0 el valor en EUR al ser EARN
                    header.insert("FEE_AMOUNT", index);

                if(columns[index] == "TO CURRENCY")
                    header.insert("PAIR2", index);
                if(columns[index] == "TO AMOUNT")
                    header.insert("PAIR2_AMOUNT", index);

                if(columns[index] == "FEE CURRENCY")
                    header.insert("FIAT_CURRENCY", index);
                if(columns[index] == "TO RATE EUR")
                    header.insert("PAIR2_AMOUNT_FIAT", index);
            }
        }
        else if(firstLine > 1)
        {
            auto lineV = line.split(',');
            if(lineV[header["TYPE"]] == "earn")
            {
                qDebug() << "Operation: " << lineV[header["PAIR1"]] << " " << lineV[header["PAIR2"]] << " " << lineV[header["PAIR2_AMOUNT_FIAT"]].toDouble();
                operations_.push_back(std::make_shared<Operation>(0, 
                            lineV[header["FEE"]],  // Pair 1 Coin
                            lineV[header["PAIR2"]], // Pair 2 Coin
                            0.0, // Pair1Amount
                            1.0, // Pair1AmountFiat
                            lineV[header["PAIR2_AMOUNT"]].toDouble(), // Pair2Amount
                            lineV[header["PAIR2_AMOUNT_FIAT"]].toDouble(), // Pair2AmountFiat
                            0.0, // Fees Amount
                            1.0, // Fees Amount fiat
                            "Accepted", // Status
                            cnvDateTime2Str(datetimeStrToDatetime(lineV[header["DATE"]])), // Date //TODO: This variable must be dateTime type
                            "", // Description 
                            "Earn", // Type
                            lineV[header["PAIR2_AMOUNT_FIAT"]].toDouble()*lineV[header["PAIR2_AMOUNT"]].toDouble() // Ganancia
                            ));
            }
        }

        if(firstLine < 3)
            firstLine++;
    }

    if(operations_.size() > 0)
        return operations_;
    else
        return std::nullopt;
}

QDateTime B2m::datetimeStrToDatetime(QByteArray dtimeStr)
{
    return QDateTime::fromString(dtimeStr, Qt::DateFormat::ISODateWithMs); // Date
}