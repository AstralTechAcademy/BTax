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

Broker::Broker(const QString& server, const QString& version, const QString database, QObject *parent)
{

    std::cout << "File: Broker.cpp Func: Broker " << QSysInfo::kernelType().toStdString() << std::endl;
    version_ = version;
    database_ = database;
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


};


bool Broker::openDatabase (void)
{
    QFuture<bool> future = QtConcurrent::run([&](){
        emit connectingDatabase();
        dbOpened_ = DBRemote::GetInstance()->openDatabase();
        std:: cout << "DB Server Opened: " <<  dbOpened_ << std::endl;
        server_ = DBRemote::GetInstance()->getServer();
        if(!dbOpened_)
        {
                emit notOpened();
                return false;
        }
        else
        {
                emit opened();
                return true;
        }
    });

    return future.result();
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

QString Broker::getDatabase(void) const
{
    return database_;
}
bool Broker::isOpened(void) const
{
    return dbOpened_;
}


void Broker::load(void)
{
    qDebug() << "Loading";
    BrokerManager::getInstance()->load();
    emit loaded();
}
