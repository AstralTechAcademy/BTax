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
    parseHeader(csv);

    while (!csv.atEnd()) {
        QByteArray line = csv.readLine();
        auto lineV = line.split(separator_);
        //Defaults
        double pair1Amount = 0.0;
        double pair1AmountFiat = 1.0;
        double pair2AmountFiat = 0.0;

        if(lineV[header_[EN_COLUMN_NAMES::TYPE]] == "POS savings interest")
        {
            qDebug() << "Operation: " << lineV[header_[EN_COLUMN_NAMES::PAIR2]] << " " << lineV[header_[EN_COLUMN_NAMES::PAIR2_AMOUNT]].toDouble();
            operations.push_back(std::make_shared<Operation>(0, BrokerManager::DEF_FIAT,  // Pair 1 Coin
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

    auto allOpsWithPrice = getFiatPrice(operations);
    if(operations.size() > 0 && allOpsWithPrice)
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
    QList<QString> opsWithoutPrice;
    auto coingecko = Coingecko::getInstace();

    for(auto o : operations)
    {
        auto id = coingecko->getCoinID(o->getPair2());
        if(id == std::nullopt)
            return false;
        
        qDebug() << o->getDate() << " " << o->getDateTime();

        auto price = coingecko->getPrice(id.value(), o->getDateTime());

        if(price == std::nullopt)
            opsWithoutPrice.push_back(id.value() + " " + o->getDateTime().toString() );
        
        qDebug() << "Price: " << id.value()  << " " << o->getDateTime() << " " << price.value();
        o->setPair2AmountFiat(price.value());

    }
        
    if(opsWithoutPrice.size() > 0)
    {
        qDebug() << "There are " << opsWithoutPrice.size()  << " operations without fiat price";
        for(auto l : opsWithoutPrice)
            qDebug() << l;
        return false;
    }
    else
        return true;
}

void Binance::parseHeader(QFile& csv)
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
}