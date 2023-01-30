//
// Created by gabridc on 19/4/22.
//

#include "B2mV2.h"
#include <QFile>
#include <QTimeZone>

B2mV2::B2mV2(void){};

QDateTime B2mV2::datetimeStrToDatetime(QByteArray dtimeStr)
{
    auto dateTime = QDateTime::fromString(dtimeStr, Qt::DateFormat::ISODateWithMs);
    dateTime.setTimeZone(QTimeZone("Europe/London"));
    qDebug() << dateTime;
    qDebug() << dateTime.toTimeZone(QTimeZone("Europe/Madrid")); // Date
    return dateTime.toTimeZone(QTimeZone("Europe/Madrid")); // Date
}

bool B2mV2::parseHeader(QFile& csv)
{
    bool error = true;
    QByteArray line = csv.readLine();
    line = csv.readLine();
    line.chop(2);
    auto columns = line.split(separator_);
    for(auto index = 0; index < columns.size(); index++)
    {
        if(columns[index] == "Fecha")
            header_.insert(EN_COLUMN_NAMES::DATE, index);
        if(columns[index] == "Grupo")
            header_.insert(EN_COLUMN_NAMES::TYPE, index);
        if(columns[index] == "Moneda de origen") // Se coge usando la columna de fees. Moneda por defecto
            header_.insert(EN_COLUMN_NAMES::PAIR1, index);
        if(columns[index] == "Cantidad de origen") // Se coge un valor 0 porque siempre es 0 el valor en EUR al ser EARN
            header_.insert(EN_COLUMN_NAMES::PAIR1_AMOUNT, index);
        if(QString(columns[index]).contains("Comisi")) // Se coge un valor 0 porque siempre es 0 el valor en EUR al ser EARN
            header_.insert(EN_COLUMN_NAMES::FEE_AMOUNT, index);
        if(columns[index] == "Moneda de destino")
            header_.insert(EN_COLUMN_NAMES::PAIR2, index);
        if(columns[index] == "Cantidad de destino")
            header_.insert(EN_COLUMN_NAMES::PAIR2_AMOUNT, index);
        if(QString(columns[index]).contains("Moneda de la comis"))
        {
            header_.insert(EN_COLUMN_NAMES::FIAT_CURRENCY, index);
            header_.insert(EN_COLUMN_NAMES::FEE, index);
        }

    }

    error = header_.size() != 9;
    if(error)
        qDebug() << "[Error][B2mV2::parseHeader] Header does not match.";
    return error;
}

bool B2mV2::parseBody(QFile& csv)
{
    while (!csv.atEnd()) {
        QByteArray line = csv.readLine();
        line.chop(2);
        if(line.isEmpty() == false and line.size() > 1 and line.contains(separator_))
        {
            qsizetype  open = line.indexOf('"');
            qsizetype  commaPos = line.indexOf(',', open);
            qsizetype  close = line.indexOf('"', open+1);

            line.replace(commaPos, 1, QByteArrayView("."));
            line.remove(open , 1);
            line.remove(close-1 , 1);
            auto lineV = line.split(separator_);
            if(lineV.size() > 0 && lineV[header_[EN_COLUMN_NAMES::TYPE]] == "earn")
            {
                //Defaults
                double pair1Amount = 0.0;
                double pair1AmountFiat = 1.0;
                double pair2AmountFiat = 0.0;
                qDebug() << "Operation: " << lineV[header_[EN_COLUMN_NAMES::PAIR1]] << " " << lineV[header_[EN_COLUMN_NAMES::PAIR2]] << " " << lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble();
                operations_.push_back(std::make_shared<Operation>(0, 
                            BrokerManager::DEF_FIAT,  // Pair 1 Coin
                            lineV[header_[EN_COLUMN_NAMES::PAIR2]], // Pair 2 Coin
                            pair1Amount,
                            pair1AmountFiat,
                            lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble(), // Pair2Amount
                            pair2AmountFiat, // Pair2AmountFiat
                            0.0, // Fees Amount
                            1.0, // Fees Amount fiat
                            "Accepted", // Status
                            cnvDateTime2StrFormat(datetimeStrToDatetime(lineV[header_[EN_COLUMN_NAMES::DATE]]), EN_DateFormat::DMYhms), // Date
                            "", // Description 
                            "Earn", // Type
                            pair2AmountFiat*lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble() // Ganancia
                            ));
            }
        }
    }

    auto allOpsWithPrice = getFiatPrice(operations_);
    calcFiatReward(operations_);
    if(operations_.size() > 0 && allOpsWithPrice)
        return true;
    else
        return false;

}