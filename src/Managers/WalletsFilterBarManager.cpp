#include "WalletsFilterBarManager.h"
#include "Logger.h"

void WalletsFilterBarManager::filter(QString filterName, QString value)
{
    if(filterName == "AssetType")
        assetTypeFilter_.update(value);
       
    if(filterName == "AssetCategory")
    {
        assetCategoryFilter_.update(value);
        emit assetCategorySelected(value);
    }
        
    

}

const QList<WalletsModel::AssetType> WalletsFilterBarManager::getAssetTypes() const noexcept 
{
   /*     enum class AssetType {
        ALL = 0,
        CRYPTO,
        DEBT,
        ETF,
        FIAT,
        FUND,
        REAL_STATE,
        SHARE
    };  */
    QList<WalletsModel::AssetType> selected;
    auto assets = assetTypeFilter_.get();

    for(auto ass : assets)
    {
        if (ass == "debt")
            selected.push_back(WalletsModel::AssetType::DEBT);
        else if (ass == "crypto")
            selected.push_back(WalletsModel::AssetType::CRYPTO);
        else if (ass == "fund")
            selected.push_back(WalletsModel::AssetType::FUND);
        else if (ass == "share")
            selected.push_back(WalletsModel::AssetType::SHARE);
        else if (ass == "real state")
            selected.push_back(WalletsModel::AssetType::REAL_STATE);
        else if (ass == "fiat")
            selected.push_back(WalletsModel::AssetType::FIAT);
        else if (ass == "etf")
            selected.push_back(WalletsModel::AssetType::ETF);
        else
            LOG_ERROR("Asset type %s not found", qPrintable(ass));
    }





    return selected;
}