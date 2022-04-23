//
// Created by gabridc on 19/4/22.
//

#include "Cryptocom.h"

Cryptocom::Cryptocom(void){};


std::optional<QList<std::shared_ptr<Operation>>> Cryptocom::import(const QString& csvPath)
{
    QFile csv(csvPath);
    if(csv.exists() == false)
        return std::nullopt;
    else
    {
        return parse(csv);
    }
}

QList<std::shared_ptr<Operation>> Cryptocom::parse(QFile& csv)
{
    csv.open(QIODevice::ReadOnly);
    uint8_t firstLine = 0;
    QList<std::shared_ptr<Operation>> operations;

    QMap<QString, int> header;
    while (!csv.atEnd()) {
        QByteArray line = csv.readLine();

        if(firstLine == 0) // HEADER
        {
            auto columns = line.split(',');
            for(auto index = 0; index < columns.size(); index++)
            {

                if(columns[index] == "Timestamp (UTC)")
                    header.insert("DATE", index);
                if(columns[index] == "Transaction Description")
                    header.insert("TYPE", index);

                if(columns[index] == "Currency")
                    header.insert("PAIR1", index);
                if(columns[index] == "Amount")
                    header.insert("PAIR1_AMOUNT", index);


                if(columns[index] == "To Currency")
                    header.insert("PAIR2", index);
                if(columns[index] == "To Amount")
                    header.insert("PAIR2_AMOUNT", index);

                if(columns[index] == "Native Currency")
                    header.insert("FIAT_CURRENCY", index);
                if(columns[index] == "Native Amount")
                    header.insert("FIAT_AMOUNT", index);
            }
        }
        else
        {
            auto lineV = line.split(',');
            if(lineV[header["TYPE"]] == "Card Cashback")
            {
                operations.push_back(std::make_shared<Operation>(0, lineV[header["FIAT_CURRENCY"]], lineV[header["PAIR1"]], 0.0, 1.0,
                                                                 lineV[header["PAIR1_AMOUNT"]].toDouble(),
                                                                 lineV[header["FIAT_AMOUNT"]].toDouble() / lineV[header["PAIR1_AMOUNT"]].toDouble(),
                                                                 0.0, 1.0, "Accepted",
                                                                 QDateTime::fromString(lineV[header["DATE"]], Qt::DateFormat::ISODate).toString(),
                                                                 "", "Cashback", lineV[header["FIAT_AMOUNT"]].toDouble()));
            }
        }

        firstLine = 1;
    }
    return operations;
}