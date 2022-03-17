//
// Created by gabridc on 26/8/21.
//

#include "Broker.h"
#include <QSysInfo>
#include <iostream>

Broker::HOSTS Broker::host_  = Broker::HOSTS::LINUX;
QString Broker::server_ = "None";
QString Broker::version_ = "0.0.0";

Broker::Broker(const QString& server, const QString& version, QObject *parent) : QObject(parent)
{
    std::cout << "File: Broker.cpp Func: Broker " << QSysInfo::kernelType().toStdString() << std::endl;
    server_ = server;
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
};


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
