//
// Created by gabridc on 26/8/21.
//

#include "Broker.h"
#include <QSysInfo>
#include <iostream>
#include <QtConcurrent/QtConcurrent>

Broker::HOSTS Broker::host_  = Broker::HOSTS::LINUX;
QString Broker::server_ = "None";
QString Broker::version_ = "0.0.0";
bool Broker::dbOpened_ = false;

Broker::Broker(const QString& server, const QString& version, QQmlApplicationEngine& engine, QObject *parent) : QObject(parent), engine_(engine)
{

    std::cout << "File: Broker.cpp Func: Broker " << QSysInfo::kernelType().toStdString() << std::endl;
    version_ = version;
    if(QSysInfo::productType() == "linux")
    {
        host_ = HOSTS::LINUX;
    }
    else if(QSysInfo::productType() == "windows")
    {
        host_ = HOSTS::WIN;
    }
    else if(QSysInfo::productType() == "darwin")
    {
        host_ = HOSTS::MACOSX;
    }
    else if(QSysInfo::productType() == "android")
    {
        host_ = HOSTS::ANDROIDD;
    }

    NotificationManager notificationManager;
    engine.rootContext()->setContextProperty("notificationManager", &notificationManager);
};


void Broker::openDatabase (void)
{
    QtConcurrent::run([&](){
        emit connectingDatabase();
        dbOpened_ = DBRemote::GetInstance()->openDatabase();
        std:: cout << "DB Server Opened: " <<  dbOpened_ << std::endl;
        if(!dbOpened_)
                emit notOpened();
        else
                emit opened();
        server_ = DBRemote::GetInstance()->getServer();
    });
}

int Broker::getHost(void) const
{
    std::cout << "File: Broker.cpp Func: Broker " << QSysInfo::kernelType().toStdString() << std::endl;
    return int(host_);
}

QString Broker::getServer(void) const
{
    return server_;
}
QString Broker::getVersion(void) const
{
    return version_;
}
bool Broker::isOpened(void) const
{
    return dbOpened_;
}


void Broker::load(void)
{
    qDebug() << "Loading";
    UsersModel usersModel;
    OperationsModel operationsModel;
    WalletsModel walletsModel;
    WalletsModel walletsModelAll;
    WalletsPercModel walletsPercModel;
    CoinsModel coinsModel;
    AssetTypeModel assetTypeModel;

    usersModel.setUsers();

    engine_.rootContext()->setContextProperty("operationsModel", &operationsModel);
    engine_.rootContext()->setContextProperty("walletsModel", &walletsModel);
    engine_.rootContext()->setContextProperty("walletsModelAll", &walletsModelAll);
    engine_.rootContext()->setContextProperty("walletsPercModel", &walletsPercModel);
    engine_.rootContext()->setContextProperty("usersModel", &usersModel);
    engine_.rootContext()->setContextProperty("coinsModel", &coinsModel);
    engine_.rootContext()->setContextProperty("assetTypesModel", &assetTypeModel);

    BrokerManager* brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel);
    engine_.rootContext()->setContextProperty("brokerManager", brokerManager);

    Importer *importer = Importer::getInstance(std::shared_ptr<BrokerManager>(brokerManager));
    engine_.rootContext()->setContextProperty("importer", importer);

    emit loaded();
}
