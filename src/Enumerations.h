#ifndef _ENUMERATIONS_H_
#define _ENUMERATIONS_H_
#include <QString>

enum class EN_AssetType {
    ALL = 0,
    CRYPTO,
    DEBT,
    ETF,
    FIAT,
    FUND,
    REAL_STATE,
    SHARE
};  

EN_AssetType toEN_AssetType(const QString& v);
QString toString(const EN_AssetType& v);

#endif