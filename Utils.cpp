#include "Utils.h"
#include <iostream>
#include <QTimeZone>
#include <QDate>
#include <QTime>
#include <QString>

void cnvDateTime2Str(QDateTime datetime, QString& datetimeStr)
{
     datetimeStr = QString::number(datetime.date().year()) + "/" + 
                    QString::number(datetime.date().month()) + "/" + 
                    QString::number(datetime.date().day()) + " " +
                    QString::number(datetime.time().hour()) + ":" +
                    QString::number(datetime.time().minute()) + ":" +
                    QString::number(datetime.time().second());
}

QString cnvDateTime2DMYhms(QDateTime datetime)
{
     return QString::number(datetime.date().day()) + "/" + 
                    QString::number(datetime.date().month()) + "/" + 
                    QString::number(datetime.date().year()) + " " +
                    QString::number(datetime.time().hour()) + ":" +
                    QString::number(datetime.time().minute()) + ":" +
                    QString::number(datetime.time().second());
}

QString cnvDateTime2StrFormat(QDateTime datetime, EN_DateFormat format)
{
    std::cout << datetime.toString().toStdString() << std::endl; 
    QString str;
    switch(format)
    {
        case EN_DateFormat::ISO_8601:
            str = "1970-01-01 00:00:00";
            break;
        case EN_DateFormat::DMYhms:
            str = cnvDateTime2DMYhms(datetime);
            break;
        case EN_DateFormat::QT:
        default:
            str = datetime.toString();
            break;
    }

    return str;
}

QDateTime cnvDBStr2DateTime(QString datetime)
{
     return QDateTime::fromString(datetime);
}

void cnvStrToDateTime(QString& datetimeStr, QDateTime& dateTime)
{
    if(datetimeStr == "")
        datetimeStr = QDateTime::currentDateTime().toString();
    else if(datetimeStr.contains(" ") and datetimeStr.contains("/"))
    {

        dateTime.setDate(QDate(datetimeStr.split(" ")[0].split("/")[2].toInt(),
                               datetimeStr.split(" ")[0].split("/")[1].toInt(),
                               datetimeStr.split(" ")[0].split("/")[0].toInt()));
        dateTime.setTime(QTime(datetimeStr.split(" ")[1].split(":")[0].toInt(),
                               datetimeStr.split(" ")[1].split(":")[1].toInt(),
                               datetimeStr.split(" ")[1].split(":")[2].toInt()));
        datetimeStr = dateTime.toString();
        //dateTime.setTimeZone(QTimeZone::utc());
    }
    else if(datetimeStr.contains(" ") and datetimeStr.split(" ").size() == 5)
    {
        dateTime = QDateTime::fromString(datetimeStr);
    }
}

QString dateTimeToUTC0(QDateTime time, QString exchange)
{
    if (exchange == "B2M" or exchange == "Bitpanda")
    {
        QTimeZone zone("Europe/Madrid");
        time.setTimeZone(zone);
        std::cout << time.toString(Qt::ISODateWithMs).replace('T', ' ').toStdString() << " ";
        std::cout << time.toUTC().toString(Qt::ISODateWithMs).replace('T', ' ').toStdString() << std::endl;
        return time.toUTC().toString(Qt::ISODateWithMs).replace('T', ' ').replace('Z', "");
    }
    else if(exchange == "Binance" or exchange == "Crypto" or exchange == "Coinbase")
    {
        // No conversion required
        return time.toString(Qt::ISODateWithMs).replace('T', ' ').replace('Z', "");
    }
    else
    {
        // No conversion required
        return time.toString(Qt::ISODateWithMs).replace('T', ' ').replace('Z', "");
    }
}

QString EN_Exchange2String(EN_Exchange exh)
{
    switch (exh)
    {
    case EN_Exchange::B2M:
        return "B2M";
        break;
    case EN_Exchange::BINANCE:
        return "Binance";
        break;
    case EN_Exchange::CRYPTO:
        return "Crypto";
        break;
    case EN_Exchange::BITPANDA:
        return "Bitpanda";
        break;     
    default:
        break;
    }
}
