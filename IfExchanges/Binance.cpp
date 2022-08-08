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

    QMap<QString, int> header;
    while (!csv.atEnd()) {
        QByteArray line = csv.readLine();
        if(firstLine == 0) // HEADER
        {
            auto columns = line.split(',');
            for(auto index = 0; index < columns.size(); index++)
            {

                if(columns[index] == "UTC_Time")
                    header.insert("DATE", index);
                if(columns[index] == "Operation")
                    header.insert("TYPE", index);

                if(columns[index] == "Coin")
                    header.insert("PAIR2", index);
                if(columns[index] == "Change")
                    header.insert("PAIR2_AMOUNT", index);

            }
        }
        else
        {
            auto lineV = line.split(',');
            qDebug() << lineV;

            //Defaults
            double pair1Amount = 0.0;
            double pair1AmountFiat = 1.0;
            double pair2AmountFiat = 0.0;

            if(lineV[header["TYPE"]] == "POS savings interest")
            {
                auto walletID = DBLocal::GetInstance()->getWalletID(DBLocal::GetInstance()->getUserID("gabridc"), "Binance", BrokerManager::DEF_FIAT_ID);

                if (walletID == 0)
                {
                    qDebug() << "File: Binance.cpp Function: parse Description: No se encuentra la wallet Binance:EUR en el usuario gabridc";
                    return std::nullopt;
                }
                operations.push_back(std::make_shared<Operation>(0, BrokerManager::DEF_FIAT, lineV[header["PAIR2"]], pair1Amount, pair1AmountFiat,
                                                                 lineV[header["PAIR2_AMOUNT"]].toDouble(),
                                                                 pair2AmountFiat,
                                                                 0.0, 1.0, "Accepted",
                                                                 QDateTime::fromString(lineV[header["DATE"]], Qt::DateFormat::ISODate).toString(),
                                                                 "", "Earn", pair2AmountFiat*lineV[header["PAIR2_AMOUNT"]].toDouble()));
            }
        }

        firstLine = 1;
    }

    if(operations.size() > 0)
        return operations;
    else
        return std::nullopt;
}