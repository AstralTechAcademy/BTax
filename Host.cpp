#include "Host.h"


Host *Host::instance_ = nullptr;
Host::Host(void)
{
    QString kernel = QSysInfo::kernelType();
    QString type = QSysInfo::productType();
    qDebug() << kernel;
    if(kernel == "linux")
    {
        dirPath_ = LinuxBtaxPath;
        configPath_ = LinuxConfigPath;
        logPath_ = LinuxLogPath;
    }
    else if(kernel == "winnt")
    {
        dirPath_ = WinBtaxPath;
        configPath_ = WinConfigPath;
        logPath_ = WinLogPath;
    }
    else if(kernel == "darwin")
    {
        dirPath_ = DarwinBtaxPath;
        configPath_ = DarwinConfigPath;
        logPath_ = DarwinLogPath;
    }

    create();
    printPaths();
}

const QString & Host::getDirPath() const
{
    return dirPath_;
}
const QString & Host::getConfigPath() const
{
    return configPath_;
}
const QString & Host::getLogPath() const
{
    return logPath_;
}

void Host::create(void) const noexcept
{
    if(QDir(dirPath_).exists() == false)
        QDir().mkpath(dirPath_);
    if(QDir(logPath_).exists() == false)
        QDir().mkpath(logPath_);
}

void Host::printPaths(void)
{
    qDebug() << getDirPath();
    qDebug() << getConfigPath();
    qDebug() << getLogPath();
}