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

    qDebug() << root["database"].toObject().keys();

    return validate(root);

    return true;
}  

bool Config::validate(const QJsonObject& root) noexcept
{
    QString stage = "test";
#ifdef GTEST
    stage = "test";
#elif MANUAL_TEST
    stage = "manual_test";
#elif OFICIAL    
    stage = "production";
#endif

    QJsonObject content;
    if(root.keys().indexOf("database") == -1 or root.keys().indexOf("database") == -1)
        return false;
    
    if(root["yfinance_api"].toObject().keys().indexOf(stage) == -1)
        return false; 

    if(root["database"].toObject().keys().indexOf(stage) == -1)
        return false;

    content = root["database"][stage].toObject();
    if(content.keys().indexOf("host") == -1 or
       content.keys().indexOf("port") == -1 or
       content.keys().indexOf("username") == -1 or
       content.keys().indexOf("password") == -1 or
       content.keys().indexOf("database") == -1)
        return false;
    
    server_ = content["host"].toString();
    port_ = content["port"].toInt();
    username_ = content["username"].toString();
    password_ = content["password"].toString();
    databasename_ = content["database"].toString();
    
    qDebug() << "Database\n\t" << server_ << " " << port_ << username_ << " " << password_ << " " << databasename_; 

    content = root["yfinance_api"][stage].toObject();
    if(content.keys().indexOf("host") == -1 or
       content.keys().indexOf("port") == -1)
        return false;
 
    yfServer_ = content["host"].toString();
    yfPort_ = content["port"].toInt();

    qDebug() << "YFinance\n\t" <<yfServer_ << " " << yfPort_; 


return true;

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