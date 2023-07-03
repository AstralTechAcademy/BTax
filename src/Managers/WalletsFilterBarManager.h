//
// Created by gabridc on 3/11/21.
//

#ifndef WALLETS_FILTER_BAR_MANAGER_H
#define WALLETS_FILTER_BAR_MANAGER_H
#include <QObject>
#include <QMap>
#include <QString>
#include <QDate>
#include <variant>
#include "Filters/SelectionMultipleFilter.h"
#include "WalletsModel.h"

class WalletsFilterBarManager : public QObject
{
    Q_OBJECT

    public:
    static WalletsFilterBarManager* getInstance(void) {
        if (!instance_)
            instance_ = new WalletsFilterBarManager();
        return instance_;
    }

    const QList<WalletsModel::AssetType> getAssetTypes() const noexcept;

public slots:
   // void filter(QString filterName, QDate value1, QDate value2);
    void filter(QString filterName, QString value);
    
signals:
    void apply();
    void assetCategorySelected(const QString& category);

private:
    // Singletone
    WalletsFilterBarManager (QObject *parent = 0) 
    {
        assetTypeFilter_ = SelectionMultipleFilter("AssetType");
        assetCategoryFilter_ = SelectionMultipleFilter("AssetCategory");
    };
    inline static WalletsFilterBarManager* instance_;

    // Filters
    SelectionMultipleFilter assetTypeFilter_;
    SelectionMultipleFilter assetCategoryFilter_;
};

#endif