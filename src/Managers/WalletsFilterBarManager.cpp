#include "WalletsFilterBarManager.h"
#include "Logger.h"

void WalletsFilterBarManager::filter(QString filterName, QString value)
{
    if(filterName == "AssetType")
        assetTypeFilter_.update(value);
       
    if(filterName == "AssetCategory")
    {
        assetTypeFilter_.clear();
        assetCategoryFilter_.update(value);
        emit assetCategorySelected(value);
    }
        
    

}

void WalletsFilterBarManager::clean()
{
    assetTypeFilter_.clear();
    assetCategoryFilter_.clear();
}

const QList<EN_AssetType> WalletsFilterBarManager::getAssetTypes() const noexcept 
{
    QList<EN_AssetType> selected;
    auto assets = assetTypeFilter_.get();

    for(auto ass : assets)
    {
        if (ass == "debt")
            selected.push_back(EN_AssetType::DEBT);
        else if (ass == "crypto")
            selected.push_back(EN_AssetType::CRYPTO);
        else if (ass == "fund")
            selected.push_back(EN_AssetType::FUND);
        else if (ass == "share")
            selected.push_back(EN_AssetType::SHARE);
        else if (ass == "real state")
            selected.push_back(EN_AssetType::REAL_STATE);
        else if (ass == "fiat")
            selected.push_back(EN_AssetType::FIAT);
        else if (ass == "etf")
            selected.push_back(EN_AssetType::ETF);
        else
            LOG_ERROR("EN_AssetType type %s not found", qPrintable(ass));
    }

    return selected;
}