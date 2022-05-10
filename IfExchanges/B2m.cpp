//
// Created by gabridc on 19/4/22.
//

#include "B2m.h"
#include <QFile>

B2m::B2m(void){};


std::optional<QList<std::shared_ptr<Operation>>> B2m::import(const QString& csvPath)
{
    std::cout << "Import";
    QFile csv(csvPath);

    if(csv.exists() == false)
        return std::nullopt;
    else
    {
        return parse(csv);
    }
}

std::optional<QList<std::shared_ptr<Operation>>> B2m::parse(QFile& csv)
{
    csv.open(QIODevice::ReadOnly);
    uint8_t firstLine = 0;
    QList<std::shared_ptr<Operation>> operations;

    QMap<QString, int> header;
    while (!csv.atEnd()) {
        QByteArray line = csv.readLine();
        if(firstLine == 1) // HEADER
        {
            auto columns = line.split(',');
            for(auto index = 0; index < columns.size(); index++)
            {

                if(columns[index] == "DATE UNIX")
                    header.insert("DATE", index);
                if(columns[index] == "METHOD")
                    header.insert("TYPE", index);

                if(columns[index] == "FEE CURRENCY") // Se coge usando la columna de fees. Moneda por defecto
                    header.insert("PAIR1", index);
                if(columns[index] == "FEE AMOUNT") // Se coge un valor 0 porque siempre es 0 el valor en EUR al ser EARN
                    header.insert("PAIR1_AMOUNT", index);


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
                //std::cout << line.toStdString() << std::endl;
                operations.push_back(std::make_shared<Operation>(0, lineV[header["PAIR1"]], lineV[header["PAIR2"]], 0.0, 1.0,
                                                                 lineV[header["PAIR2_AMOUNT"]].toDouble(),
                                                                 lineV[header["PAIR2_AMOUNT_FIAT"]].toDouble(),
                                                                 0.0, 1.0, "Accepted",
                                                                 QDateTime::fromString(lineV[header["DATE"]], Qt::DateFormat::ISODate).toString(),
                                                                 "", "Earn",
                                                                 lineV[header["PAIR2_AMOUNT_FIAT"]].toDouble()*lineV[header["PAIR2_AMOUNT"]].toDouble()));
            }
        }

        if(firstLine < 3)
            firstLine++;
    }

    if(operations.size() > 0)
        return operations;
    else
        return std::nullopt;
}