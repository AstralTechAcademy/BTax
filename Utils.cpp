#include "Utils.h"
#include <iostream>
#include <QTimeZone>
#include <QDate>
#include <QTime>
#include <QString>
#include <QLocale>

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
    QString str;
    switch(format)
    {
        case EN_DateFormat::ISO_8601:
            str = "1970-01-01 00:00:00";
            break;
        case EN_DateFormat::DMYhms:
            str = BTime::toString(datetime, QLocale::Spanish, EN_DateFormat::DMYhms);
            break;
        case EN_DateFormat::QT:
        default:
            str = BTime::toString(datetime);
            break;
    }

    return str;
}

QDateTime cnvDBStr2DateTime(QString datetime)
{
    auto locale = QLocale(QLocale::Spanish);
    QString format = "ddd. MMM. d hh:mm:ss yyyy";
    auto r = locale.toDateTime(datetime, format);
    return r;
}

void cnvStrToDateTime(QString& datetimeStr, QDateTime& dateTime)
{
    if(datetimeStr == "")
        datetimeStr =   BTime::toString(QDateTime::currentDateTime());
    else if(datetimeStr.contains(" ") and datetimeStr.contains("/"))
    {

        dateTime.setDate(QDate(datetimeStr.split(" ")[0].split("/")[2].toInt(),
                               datetimeStr.split(" ")[0].split("/")[1].toInt(),
                               datetimeStr.split(" ")[0].split("/")[0].toInt()));
        dateTime.setTime(QTime(datetimeStr.split(" ")[1].split(":")[0].toInt(),
                               datetimeStr.split(" ")[1].split(":")[1].toInt(),
                               datetimeStr.split(" ")[1].split(":")[2].toInt()));
        datetimeStr = BTime::toString(dateTime);
    }
    else if(datetimeStr.contains(" ") and datetimeStr.split(" ").size() == 5)
    {
        dateTime = BTime::toDateTime(datetimeStr, QLocale::Spanish, EN_DateFormat::QT);
        //dateTime = QDateTime::fromString(datetimeStr);
    }
}

QString dateTimeToUTC0(QDateTime time, QString exchange)
{
    if (exchange == "B2M" or exchange == "Bitpanda")
    {
        QTimeZone zone("Europe/Madrid");
        time.setTimeZone(zone);
        return time.toUTC().toString(Qt::ISODateWithMs).replace('T', ' ').replace('Z', "");
    }
    else if(exchange == "Binance" or exchange == "Crypto" or exchange == "Coinbase")
    {
        // No conversion required
        auto locale = QLocale(QLocale::Spanish);
        QString format = "yyyy-MM-dd hh:mm:ss.zzz";
        return locale.toString(time, format);
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
    case EN_Exchange::B2M_V2:
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


QString BTime::toString(QDateTime time, QLocale::Language lang, EN_DateFormat format)
{
    QString f = "ddd. MMM. d hh:mm:ss yyyy";
    switch(format)
    {
        case EN_DateFormat::ISO_8601: 
            f = "yyyy-MM-dd hh:mm:ss";
            break;
        case EN_DateFormat::DMYhms: 
            f = "dd/MM/yyyy hh:mm:ss";
            break;   
        case EN_DateFormat::QT:
        default:
            f = "ddd. MMM. d hh:mm:ss yyyy";
            break;                  
    }

    return QLocale(lang).toString(time, f);
}

QDateTime BTime::toDateTime(QString time, QLocale::Language lang, EN_DateFormat format)
{
    QString f = "ddd. MMM. d hh:mm:ss yyyy";
    switch(format)
    {
        case EN_DateFormat::ISO_8601: 
            f = "yyyy-MM-dd hh:mm:ss";
            break;
        case EN_DateFormat::DMYhms: 
            f = "dd/MM/yyyy hh:mm:ss";
            break;   
        case EN_DateFormat::QT:
        default:
            f = "ddd. MMM. d hh:mm:ss yyyy";
            break;                  
    }

    return QLocale(lang).toDateTime(time, f);
}
