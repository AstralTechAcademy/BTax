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


    static Importer* getInstance(std::shared_ptr<BrokerManager> brokerManager) {
        if(instance_ == nullptr)
            instance_ = new Importer(brokerManager);
        return instance_;

    }

public slots:
    bool import(const QString& exchange, const QString& csvPath) noexcept;

    QString  opsAlrdyAdded(void)
    {
        QString operations;
        for(auto op : opsAlrdyAdded_)
        {
            operations += op->getPair1() + " " + op->getPair2()  + " " + QString::number(op->getPair2Amount()) + " " +  op->getDate() +  " " + op->getType() + "\n";
        }
        return operations;
    }
    QString  opsWithError(void)
    {
        QString operations;
        for(auto op : opsWithError_)
        {
            operations += op->getPair1() + " " + op->getPair2()  +  " " + QString::number(op->getPair2Amount()) + " " + op->getDate() +  " " + op->getType() + "\n";
        }
        return operations;
    }
    QString opsAdded(void)
    {
        QString operations;
        for(auto op : opsAdded_)
        {
            operations += op->getPair1() + " " + op->getPair2()  +  " "  + QString::number(op->getPair2Amount()) + " " +  op->getDate() +  " " + op->getType() +  "\n";
        }
        return operations;
    }

    int operationsSize(void)
    {
        return operations_->size();
    }

    int opsAddedSize(void)
    {
        return opsAdded_.size();
    }

    int opsWithErrorSize(void)
    {
        return opsWithError_.size();
    }

    int opsAlrdyAddedSize(void)
    {
        return opsAlrdyAdded_.size();
    }

private:
    std::shared_ptr<BrokerManager> brokerManager_;
    std::optional<QList<std::shared_ptr<Operation>>> operations_ = std::nullopt;
    QList<std::shared_ptr<Operation>> opsAlrdyAdded_;
    QList<std::shared_ptr<Operation>> opsWithError_;
    QList<std::shared_ptr<Operation>> opsAdded_;
    inline static Importer* instance_ = nullptr;

};


#endif //BROKER_IMPORTER_H
