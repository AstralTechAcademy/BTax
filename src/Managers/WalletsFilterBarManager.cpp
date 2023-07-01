#include "WalletsFilterBarManager.h"


void WalletsFilterBarManager::filter(QString filterName, QString value)
{
    if(filterName == "AssetType")
        assetTypeFilter_.update(value);
    if(filterName == "AssetCategory")
        assetCategoryFilter_.update(value);
    
    emit assetCategorySelected(value);
}