#include "Enumerations.h"
#include "logger.h"

EN_AssetType toEN_AssetType(const QString& v)
{
    if(v == "share") return EN_AssetType::SHARE;
    else if(v == "debt") return EN_AssetType::DEBT;
    else if(v == "etf") return EN_AssetType::ETF;
    else if(v == "fiat") return EN_AssetType::FIAT;
    else if(v == "fund") return EN_AssetType::FUND;
    else if(v == "real state") return EN_AssetType::REAL_STATE;
    else if(v == "crypto") return EN_AssetType::CRYPTO;
    else LOG_ERROR("Conversion error %s to EN_AssetType", qPrintable(v)); throw;
}

QString toString(const EN_AssetType& v)
{
    switch(v)
    {
        case EN_AssetType::CRYPTO:
            return QString("crypto");
            break;
        case EN_AssetType::DEBT:
            return QString("debt");
            break;
        case EN_AssetType::ETF:
            return QString("etf");
            break;
        case EN_AssetType::FIAT:
            return QString("fiat");
            break;
        case EN_AssetType::FUND:
            return QString("fund");
            break;
        case EN_AssetType::REAL_STATE:
            return QString("real state");
            break;
        case EN_AssetType::SHARE:
            return QString("share");
            break;
        case EN_AssetType::ALL:
        default:
            return QString("all");
            break;
    }
}