//
// Created by gabridc on 26/8/21.
//

#include "Broker.h"
#include <QSysInfo>
#include <iostream>
#include <QtConcurrent/QtConcurrent>
#include "Host.h"

QString Broker::server_ = "None";
QString Broker::version_ = "0.0.0";
bool Broker::dbOpened_ = false;

using namespace Btax;

Broker::Broker(const QString& server, const QString& version, const QString database, QObject *parent)
{

    Host::getInstance();
    QByteArray ba = QString(Host::getInstance()->getLogPath() + "log.txt").toLocal8Bit();
    const char *logFPath = ba.data();

    logger_initConsoleLogger(NULL);
    logger_initFileLogger(logFPath, 0, 0);
    logger_setLevel(LogLevel_DEBUG);
    
    LOG_INFO(QSysInfo::kernelType().toStdString().c_str());
    version_ = version;
    database_ = database;
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
    LOG_INFO("Loading");
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
        LOG_ERROR("Error creating the user %s The user could be already in database", qPrintable(name));
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

