//
// Created by gabridc on 20/4/22.
//

#ifndef BROKER_IMPORTER_H
#define BROKER_IMPORTER_H
#include "ExchangeFactory.h"
#include "BrokerManager.h"
#include <QFutureWatcher>
#include "Utils.h"

class Importer : public QAbstractListModel
{

    Q_OBJECT
    Q_PROPERTY(QList<QUrl> uris MEMBER uris_ READ getUris WRITE setUri NOTIFY urisChanged)
    Q_PROPERTY(bool previewed MEMBER previewed_ READ getPreviewed WRITE setPreviewed)
public:

    enum EN_ImporterRoles {
        Path = Qt::UserRole
    };


    Importer(std::shared_ptr<BrokerManager> brokerManager, const QObject *o = nullptr);


    static Importer* getInstance(std::shared_ptr<BrokerManager> brokerManager) {
        if(instance_ == nullptr)
            instance_ = new Importer(brokerManager);
        return instance_;

    }
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    QList<QUrl> getUris(void) const noexcept;
    bool getPreviewed(void) const noexcept;
    void setUri(const QList<QUrl> uri) noexcept;
    void setPreviewed(bool p) noexcept;
    void processResult(void) noexcept;
signals:
    void urisChanged(void);
    void reading(void);
    void read(void);
    void imported(void);
    void notimported(void);


public slots:
    //bool import(const QString& exchange, const QString& csvPath) noexcept;
    void import(void) noexcept;
    void importPreview(void) noexcept;

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
        return operations_.size();
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


protected:
    // return the roles mapping to be used by QML
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    std::shared_ptr<BrokerManager> brokerManager_;
    QList<std::shared_ptr<Operation>> operations_;
    QList<std::shared_ptr<Operation>> operationsToAdd_;
    QList<std::shared_ptr<Operation>> opsAlrdyAdded_;
    QList<std::shared_ptr<Operation>> opsWithError_;
    QList<std::shared_ptr<Operation>> opsAdded_;
    inline static Importer* instance_ = nullptr;
    QList<QUrl> uris_;
    QHash<int, QByteArray> importerRoles_;
    bool previewed_ = false;

    void clear(void) noexcept;
    void printResult(void) noexcept;


#ifdef GTEST
public:
#endif
    EN_Exchange detectExchange(const QString& path);
    bool write(void) noexcept;
    bool preview(const EN_Exchange exchange, const QString csvPath) noexcept;

    std::shared_ptr<QFutureWatcher<bool>> wWrite_;
    std::shared_ptr<QFutureWatcher<bool>> wPreview_;
    std::shared_ptr<QFutureWatcher<bool>> wRead_;


    

};


#endif //BROKER_IMPORTER_H
