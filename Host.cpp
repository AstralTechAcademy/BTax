//
// Created by gabridc on 18/1/22.
//
#include "Logger.h"
#include "Host.h"
#include <QSysInfo>
#include <QDebug>


namespace Btax
{
    Host *Host::instance_ = nullptr;
    Host::Host(void)
    {
        QString kernel = QSysInfo::kernelType();
        QString type = QSysInfo::productType();
        mobile_=false;
        if(kernel == "linux")
        {
            if(type == "android")
            {
                dirPath = AndroidNeptunoPath;
                configPath = AndroidConfigPath;
                logPath_ = AndroidLogPath;
                mobile_ = true;
            }
            else
            {
                dirPath = LinuxNeptunoPath;
                configPath = LinuxConfigPath;
                logPath_ = LinuxLogPath;
            }

        }
        else if(kernel == "winnt")
        {
            dirPath = WinNeptunoPath;
            configPath = WinConfigPath;
            logPath_ = WinLogPath;
        }
        else if(kernel == "darwin")
        {
            dirPath = DarwinNeptunoPath;
            configPath = DarwinConfigPath;
            logPath_ = DarwinLogPath;
        }

        create();
    }

    const QString &Host::getDirPath() const {
        return dirPath;
    }

    const QString &Host::getConfigPath() const {
        return configPath;
    }
    const QString &Host::getLogPath() const {
        return logPath_;
    }

    void Host::create(void) const noexcept
    {
        if(QDir(dirPath).exists() == false)
            QDir().mkpath(dirPath);
        if(QDir(logPath_).exists() == false)
            QDir().mkpath(logPath_);
    }

    void Host::printPaths(void)
    {
        LOG_INFO("%s", getDirPath().toStdString());
        LOG_INFO("%s", getConfigPath().toStdString());
        LOG_INFO("%s", getLogPath().toStdString());
    }

    bool Host::isMobile(void) const
    {
        return mobile_;
    };
}
