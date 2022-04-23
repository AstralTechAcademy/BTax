//
// Created by gabridc on 20/4/22.
//

#include "Importer.h"
#include "SQLManager.h"


Importer::Importer(std::shared_ptr<BrokerManager> brokerManager, const QObject *o)
{
    brokerManager_ = brokerManager;
};

bool Importer::import(const QString& exchange, const QString& csvPath) const noexcept
{
    auto exch = ExchangeFactory::createExchange(exchange);
    auto operations = exch->import(csvPath);

    if(operations == std::nullopt)
        return false;
    else
    {
        for(auto operation : operations.value())
        {
            std::cout << "Operation " << operation->getPair2Amount() << std::endl;
            auto walletPair1 = brokerManager_->findWallet(exchange, operation->getPair1());
            auto walletPair2 = brokerManager_->findWallet(exchange, operation->getPair2());
            brokerManager_->newOperation(
                    walletPair1.value().getWalletID(),
                    walletPair2.value().getWalletID(),
                    operation->getPair1Amount(),
                    operation->getPair1AmountFiat(),
                    operation->getPair2Amount(),
                    operation->getPair2AmountFiat(),
                    operation->getFeesCoin(),
                    operation->getComision(),
                    operation->getComisionFiat(),
                    operation->getComments(),
                    operation->getType(),
                    operation->getStatus(),
                    operation->getDate());
        }
        return true;
    }
}