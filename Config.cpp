#include "Config.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Btax;

Config::Config(void)
{
}

bool Config::read(void) noexcept
{
    auto file = QFile(Host::getInstance()->getConfigPath());
    if(!file.open((QIODevice::ReadOnly)))
        return false;
    auto data = file.readAll();

    QJsonParseError jsonErr;
    auto doc = QJsonDocument::fromJson(data, &jsonErr);

    if(jsonErr.error != QJsonParseError::NoError)
        return false;

    if(!doc.isObject())
        return false;
    
    auto root = doc.object();

    qDebug() << root.keys();
}   


QString Config::getServer(void) const noexcept
{
    return server_;
}
int Config::getPort(void) const noexcept
{
    return port_;
}
QString Config::getDatabasename(void) const noexcept
{
    return databasename_;
}
QString Config::getUsername(void) const noexcept
{
    return username_;
}
QString Config::getPassword(void) const noexcept
{
    return password_;
}