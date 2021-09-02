//
// Created by gabridc on 26/8/21.
//

#include "Broker.h"
#include <QSysInfo>
#include <iostream>

Broker::HOSTS Broker::host_  = Broker::HOSTS::LINUX;

Broker::Broker(QObject *parent) : QObject(parent)
{
    std::cout << "File: Broker.cpp Func: Broker " << QSysInfo::kernelType().toStdString() << std::endl;
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
