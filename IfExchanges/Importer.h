//
// Created by gabridc on 20/4/22.
//

#ifndef BROKER_IMPORTER_H
#define BROKER_IMPORTER_H
#include "ExchangeFactory.h"
#include "BrokerManager.h"

class Importer : public QObject{

    Q_OBJECT
    Q_PROPERTY(QList<std::shared_ptr<Operation>>  opsAlrdyAdded READ opsAlrdyAdded)
    Q_PROPERTY(QList<std::shared_ptr<Operation>>  opsWithError READ opsWithError)
    Q_PROPERTY(QList<std::shared_ptr<Operation>>  opsAdded READ opsAdded)
public:
    Importer(std::shared_ptr<BrokerManager> brokerManager, const QObject *o = nullptr);
public slots:
    bool import(const QString& exchange, const QString& csvPath) noexcept;

    QList<std::shared_ptr<Operation>>  opsAlrdyAdded(void){return opsAlrdyAdded_;}
    QList<std::shared_ptr<Operation>>  opsWithError(void){return opsWithError_;}
    QList<std::shared_ptr<Operation>>  opsAdded(void) {return opsAdded_;}

private:
    std::shared_ptr<BrokerManager> brokerManager_;
    QList<std::shared_ptr<Operation>> opsAlrdyAdded_;
    QList<std::shared_ptr<Operation>> opsWithError_;
    QList<std::shared_ptr<Operation>> opsAdded_;

};


#endif //BROKER_IMPORTER_H
