//
// Created by gabridc on 20/4/22.
//

#ifndef BROKER_IMPORTER_H
#define BROKER_IMPORTER_H
#include "ExchangeFactory.h"
#include "BrokerManager.h"

class Importer : public QObject{

    Q_OBJECT
public:
    Importer(std::shared_ptr<BrokerManager> brokerManager, const QObject *o = nullptr);
public slots:
    bool import(const QString& exchange, const QString& csvPath) const noexcept;

private:
    std::shared_ptr<BrokerManager> brokerManager_;
};


#endif //BROKER_IMPORTER_H
