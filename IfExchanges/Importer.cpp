//
// Created by gabridc on 20/4/22.
//

#include "Importer.h"
#include "SQLManager.h"
#include <QDebug>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include "IMarketData/MarketDataFactory.h"


Importer::Importer(std::shared_ptr<BrokerManager> brokerManager, const QObject *o)
{

    importerRoles_[EN_ImporterRoles::Path] = "Path";
    opsAlrdyAdded_ = QList<std::shared_ptr<Operation>>();
    opsWithError_ = QList<std::shared_ptr<Operation>>();
    opsAdded_ = QList<std::shared_ptr<Operation>> ();
    brokerManager_ = brokerManager;
    wWrite_ = std::make_shared<QFutureWatcher<bool>>(new QFutureWatcher<bool>());
    wPreview_ = std::make_shared<QFutureWatcher<bool>>(new QFutureWatcher<bool>());
    wRead_ = std::make_shared<QFutureWatcher<bool>>(new QFutureWatcher<bool>());

};

QList<QUrl> Importer::getUris(void) const noexcept
{
    return uris_;
}

bool Importer::getPreviewed(void) const noexcept
{
    return previewed_;
}
void Importer::setUri(const QList<QUrl> uri) noexcept
{
    uris_.clear();
    for(auto u : uri)
        uris_.push_back(u);
    emit layoutChanged();
}

void Importer::setPreviewed(bool p) noexcept
{
    previewed_ = p;
}


void Importer::importPreview(void) noexcept
{
    LOG_INFO("importPreview called");
    clear();
    if(uris_.empty())
    {
        LOG_ERROR("No files to import"); //TODO: Emit signal
    }
        
    for (auto file : uris_)
    {
        auto path = file.toString();
        //@gadominguez: TODO: Review parh pattern windows, macos and linux
        //if(path.contains("file:///"))
        //    path = path.split("file:///")[1]; 
        //else if(path.contains("file://"))
        if(path.contains("file://"))
            path = path.split("file://")[1];
        auto exchange = detectExchange(path);
        if(exchange != EN_Exchange::UNKNOWN)
        {
            previewed_ = true;
            operationsToAdd_.clear();
            QFuture<bool> future = QtConcurrent::run(&Importer::preview, this, exchange, path);
            connect(wPreview_.get(), &QFutureWatcher<void>::finished, this, [&] {emit Importer::getInstance(brokerManager_)->read();});
            wPreview_->setFuture(future);
        }
        else
            LOG_ERROR("Unknow exchange"); //TODO: Emit signal
        

    }


}

bool Importer::preview(const EN_Exchange exchange, const QString csvPath) noexcept
{
    clear();
    auto exch = ExchangeFactory::createExchange(exchange);
    auto rOps = exch->read(csvPath);
    if(rOps == std::nullopt)
        return false;
    auto ops = operations_ = rOps.value();
    std::vector<WalletOperation> wOpsModified;
    double ganancia = 0.0;
    for(auto index = 0; index < ops.size(); index++)
    {
        ganancia +=  ops[index]->getGanancia();
        brokerManager_->newAssetIfNotExist("crypto", ops[index]->getPair1(), "#2acaea");
        brokerManager_->newAssetIfNotExist("crypto", ops[index]->getPair2(), "#2acaea");
        brokerManager_->addWalletIfNotExist(EN_Exchange2String(exchange), ops[index]->getPair1());
        brokerManager_->addWalletIfNotExist(EN_Exchange2String(exchange), ops[index]->getPair2());

        if(ops[index]->getPair2AmountFiat() != -1.0)
        {
            auto res = brokerManager_->checkDuplicity(EN_Exchange2String(exchange), ops[index]);
            if(res)
            {
                opsAlrdyAdded_.push_back(ops[index]);
            }
            else
            {
                operationsToAdd_.push_back(ops[index]);
                opsAdded_.push_back(ops[index]);
            }
        }
    }

    LOG_DEBUG("Ganancia %lf", ganancia);
    return true;
}


void Importer::import(void) noexcept
{
    LOG_INFO("import called");
    clear();
    previewed_ = false;
    QFuture<bool> future = QtConcurrent::run(&Importer::write, this);

    connect(wWrite_.get(), &QFutureWatcher<void>::finished, this, [&] {Importer::getInstance(brokerManager_)->processResult();});
    wWrite_->setFuture(future);
}

bool Importer::write(void) noexcept
{
    auto ops = operationsToAdd_;
    LOG_INFO("write called");
    std::vector<WalletOperation> wOpsModified;
    for(auto index = 0; index < ops.size();)
    {
        WalletOperation::OperationData data;
        data.fromOperation(ops[index]);
        auto res = brokerManager_->newOperation(data, wOpsModified);
        switch(res)
        {
            case static_cast<int>(BrokerManager::NewOperationRes::ADDED):
                LOG_DEBUG("Operation registered");
                opsAdded_.push_back(ops[index]);
                index++;
                break;
            case  static_cast<int>(BrokerManager::NewOperationRes::ORI_WALLET_NOT_EXIST):
                LOG_DEBUG("Origin Wallet does not exist");
                opsWithError_.push_back(ops[index]);
                index++;
                break;
            case  static_cast<int>(BrokerManager::NewOperationRes::ALREADY_ADDED):
                LOG_DEBUG("Operation duplicated");
                opsAlrdyAdded_.push_back(ops[index]);
                index++;
                break;
            case  static_cast<int>(BrokerManager::NewOperationRes::NOT_ADDED):
            default:
                LOG_DEBUG("Unknown Error");
                opsWithError_.push_back(ops[index]);
                index++;
                break;
        }
    }
    return opsWithError_.empty() == true;
}

void Importer::clear(void) noexcept
{   
    opsAdded_.clear();
    opsAlrdyAdded_.clear();
    opsWithError_.clear();
}


int Importer::rowCount(const QModelIndex &parent) const
{
        return uris_.size();
}

QVariant Importer::data(const QModelIndex &index, int role) const
{
  int row = index.row();

    // boundary check for the row
    if(row < 0 || row >= uris_.count()) {
        return QVariant();
    }

    // A model can return data for different roles.
    // The default role is the display role.
    // it can be accesses in QML with "model.display"
    switch(role) {
        case EN_ImporterRoles::Path:
            // Return the color name for the particular row
            // Qt automatically converts it to the QVariant type
            return uris_[row].fileName();
    }

    // The view asked for other data, just return an empty QVariant
    return QVariant();
}

QHash<int, QByteArray> Importer::roleNames() const
{
    return importerRoles_;
}

EN_Exchange Importer::detectExchange(const QString& path)
{
    QFile file;
    char comma = ',';
    file.setFileName(path);
    if(file.exists() == false)
    {
        LOG_ERROR("File does not exist");
        return EN_Exchange::UNKNOWN;
    }

    file.open(QIODevice::ReadOnly);

    QByteArray line = file.readLine(); //First line
    if(line.contains(comma))
    {
        // No B2M First line
        auto columns = line.split(comma);
        if(columns[0] == "User_ID")
            return EN_Exchange::BINANCE;
        
        //B2M Second line
        line = file.readLine();
        if(line.contains(comma))
        {
            columns = line.split(comma);
            if(columns[0] == "TYPE")
                return EN_Exchange::B2M;
            else if(QString(columns[0]).contains("Tipo de"))
                return EN_Exchange::B2M_V2;                
        }
    }

    file.close();
    LOG_ERROR("File format does not match");
    return EN_Exchange::UNKNOWN;


}

void Importer::printResult(void) noexcept
{
    /*qDebug() << "[INFO][Importer::printResult] Import Operation Result: " << (opsWithError_.empty() == false);
    qDebug() << "[INFO][Importer::printResult] Details: ";

    qDebug() << "Total Earn operations: " <<  operationsSize();
    qDebug() <<  "Error: "  << opsWithErrorSize() ;
    qDebug() << opsWithError();
    qDebug() <<  "Added: " << opsAddedSize() << "\n\n" << opsAdded();
    qDebug() <<  "Already: " << opsAlrdyAddedSize() ;
    qDebug() << opsAlrdyAdded();*/
}

void Importer::processResult(void) noexcept
{
    auto res = opsWithError_.empty() == true;
    if(res)
    {
        LOG_WARN("All operations added or had already been added");
        emit this->imported();
    }
    else
    {
        LOG_WARN("Some operations contains wrong data or there is unknown issue with the database");
        emit this->notimported();
    }
}