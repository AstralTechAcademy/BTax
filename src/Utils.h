#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QDateTime>
#include <QLocale>

enum EN_Exchange
{
    CRYPTO = 0,
    B2M,
    B2M_V2,
    BITPANDA,
    BINANCE,
    COINBASE,
    KRAKEN,
    UNKNOWN
};

enum EN_DateFormat
{
    ISO_8601 = 0, // YYYY-MM-DD HH:MM:SS
    DMYhms, // DD/MM/YYYY HH:MM:SS
    QT // weekday. month. day HH:MM:SS year

};

void cnvStrToDateTime(QString& datetimeStr, QDateTime& dateTime);
QString dateTimeToUTC0(QDateTime time, QString exchange);
QString EN_Exchange2String(EN_Exchange exh);
void cnvDateTime2Str(QDateTime datetime, QString& datetimeStr);
QString cnvDateTime2DMYhms(QDateTime datetime);
QString cnvDateTime2StrFormat(QDateTime datetime, EN_DateFormat format = EN_DateFormat::QT);
QDateTime cnvDBStr2DateTime(QString datetime);


class BTime
{
    public:
    static QString toString(QDateTime time, QLocale::Language lang = QLocale::Spanish, EN_DateFormat format=EN_DateFormat::QT);
    static QDateTime toDateTime(QString time, QLocale::Language lang = QLocale::Spanish, EN_DateFormat format=EN_DateFormat::QT);

};

#endif