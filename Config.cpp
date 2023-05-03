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
    stage = "gtest";
#elif MANUAL_TEST
    stage = "manual_test";
#elif OFICIAL    
    stage = "production";
#elif PROD    
    stage = "production";
#else
    stage = "test";
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
    
    dbConfig.server_ = content["host"].toString();
    dbConfig.port_ = content["port"].toInt();
    dbConfig.username_ = content["username"].toString();
    dbConfig.password_ = content["password"].toString();
    dbConfig.databasename_ = content["database"].toString();
    

    content = root["yfinance_api"][stage].toObject();
    if(content.keys().indexOf("host") == -1 or
       content.keys().indexOf("port") == -1)
        return false;
 
    yfinConfig.server_ = content["host"].toString();
    yfinConfig.port_ = content["port"].toInt();

    return true;

}

QString Config::getDbServer(void) const noexcept
{
    return dbConfig.server_;
}
int Config::getDbPort(void) const noexcept
{
    return dbConfig.port_;
}
QString Config::getDbDatabasename(void) const noexcept
{
    return dbConfig.databasename_;
}
QString Config::getDbUsername(void) const noexcept
{
    return dbConfig.username_;
}
QString Config::getDbPassword(void) const noexcept
{
    return dbConfig.password_;
}
QString Config::getYfinServer(void) const noexcept
{
    return yfinConfig.server_;
}
int Config::getYfinPort(void) const noexcept
{
    return yfinConfig.port_;
}

void Config::print(void) const noexcept
{
    qDebug() << "Database\n\t" << dbConfig.server_ << " " << dbConfig.port_ << dbConfig.username_  << " " << dbConfig.password_  << " " << dbConfig.databasename_; 
    qDebug() << "YFinance\n\t" <<yfinConfig.server_ << " " <<  yfinConfig.port_ ; 
}
