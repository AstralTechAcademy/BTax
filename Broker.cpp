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
        dbOpened_ = SQLManager::GetInstance()->openDatabase();
        std:: cout << "DB Server Opened: " <<  dbOpened_ << std::endl;
        server_ = SQLManager::GetInstance()->getServer();
        if(!dbOpened_)
        {
                emit notOpened();
                return false;
        }
        else
        {
            auto updated = SQLManager::GetInstance()->update();
            if(!updated)
            {
                emit notOpened();
                return false;
            }


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

#ifdef GTEST
BrokerManager::LoadResCode Broker::load(void)
#else
void Broker::load(void)
#endif
{
    qDebug() << "[Broker::load] Loading";
    auto res = BrokerManager::getInstance()->load();

    switch(res)
    {
        case BrokerManager::LoadResCode::OK:
            emit loaded();
            break;
        case BrokerManager::LoadResCode::NO_USERS:
            emit noUsers();
            break;
        case BrokerManager::LoadResCode::UNKNOWN:
        default:
            emit notLoaded();
            break;
    }
#ifdef GTEST
return res;
#endif

}

void Broker::newUser(const QString& name)
{
    if(!BrokerManager::getInstance()->newUser(name))
    {
        qDebug() << "[Broker::newUser] Error creating the user " << name << ". The user could be already in database.";
        emit userAlready();
        return;
    }

    auto res = BrokerManager::getInstance()->load();
    switch(res)
    {
        case BrokerManager::LoadResCode::OK:
            emit loaded();
            break;
        case BrokerManager::LoadResCode::NO_USERS:
            emit noUsers();
            break;
        case BrokerManager::LoadResCode::UNKNOWN:
        default:
            emit notLoaded();
            break;
    }
}

