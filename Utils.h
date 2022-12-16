#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QDateTime>


enum EN_Exchange
{
    CRYPTO = 0,
    B2M,
    BITPANDA,
    BINANCE,
    COINBASE,
    KRAKEN
};

void cnvStrToDateTime(QString& datetimeStr, QDateTime& dateTime);
QString dateTimeToUTC0(QDateTime time, QString exchange);
QString EN_Exchange2String(EN_Exchange exh);
void cnvDateTime2Str(QDateTime datetime, QString& datetimeStr);
QString cnvDateTime2Str(QDateTime datetime);
QString cnvDateTime2DBStr(QDateTime datetime);

#endif