//
// Created by gabridc on 20/4/22.
//

#include "Importer.h"
#include "SQLManager.h"
#include "IMarketData/MarketDataFactory.h"


Importer::Importer(std::shared_ptr<BrokerManager> brokerManager, const QObject *o)
{
    opsAlrdyAdded_ = QList<std::shared_ptr<Operation>>();
    opsWithError_ = QList<std::shared_ptr<Operation>>();
    opsAdded_ = QList<std::shared_ptr<Operation>> ();
    brokerManager_ = brokerManager;
};

bool Importer::import(const EN_Exchange exchange, const QString& csvPath) noexcept
{
    clear();
    auto exch = ExchangeFactory::createExchange(exchange);
    auto path = csvPath;
    if(csvPath.contains("file://"))
        path = csvPath.split("file://")[1];
    operations_ = exch->read(path);

    if(operations_ == std::nullopt)
        return false;

    qDebug() << "File: Importer Func: import Description: Operations imported " << operations_->size();
    auto coingecko = MarketDataFactory::createMarketData("Coingecko");
    auto ops = operations_.value();
    std::vector<WalletOperation> wOpsModified;
    for(auto index = 0; index < ops.size();)
    {
        brokerManager_->addWalletIfNotExist(EN_Exchange2String(exchange), ops[index]->getPair1());
        brokerManager_->addWalletIfNotExist(EN_Exchange2String(exchange), ops[index]->getPair2());

        if(ops[index]->getPair2AmountFiat() != -1.0)
        {
            auto res = brokerManager_->newOperation(EN_Exchange2String(exchange), ops[index], wOpsModified);
            switch(res)
            {
                case static_cast<int>(BrokerManager::NewOperationRes::ADDED):
                    qDebug() << "File: Importer Func: import Description: Operación registrada";
                    opsAdded_.push_back(ops[index]);
                    index++;
                    break;
                case  static_cast<int>(BrokerManager::NewOperationRes::ORI_WALLET_NOT_EXIST):
                    qDebug() << "File: Importer Func: import Description: Wallet origen no existe";
                    opsWithError_.push_back(ops[index]);
                    index++;
                    break;
                case  static_cast<int>(BrokerManager::NewOperationRes::NOT_ADDED):
                    qDebug() << "File: Importer Func: import Description: Error desconocido";
                    opsWithError_.push_back(ops[index]);
                    index++;
                    break;
                case  static_cast<int>(BrokerManager::NewOperationRes::ALREADY_ADDED):
                    qDebug() << "File: Importer Func: import Description: Operación duplicada";
                    opsAlrdyAdded_.push_back(ops[index]);
                    index++;
                    break;
                default:
                    qDebug() << "File: Importer Func: import Description: Error desconocido";
                    opsWithError_.push_back(ops[index]);
                    index++;
                    break;
            }
        }
    }

    // Return TRUE todas las transacciones han sido añadidas o ya estaban en la BD
    return opsWithError_.empty() == false ? true : false;

}

void Importer::clear(void) noexcept
{   
    opsAdded_.clear();
    opsAlrdyAdded_.clear();
    opsWithError_.clear();
}