#ifndef CONFIG_H
#define CONFIG_H
#include "Host.h"
#include <QString>

struct dbConn
{
    QString server_ = "";
    QString yfServer_ = "";
    QString databasename_ = "";
    QString username_ = "";
    QString password_ = "";
    int port_ = 0;
};

struct yfinanceConn
{
    QString server_ = "";
    int port_ = 0;

};
class Config
{

public:
    static Config* getInstance(void)
    {
        if(instance_ == nullptr)
            instance_ = new Config();
        return instance_;
    }

    QString getDbServer(void) const noexcept;
    int getDbPort(void) const noexcept;
    QString getDbDatabasename(void) const noexcept;
    QString getDbUsername(void) const noexcept; 
    QString getDbPassword(void) const noexcept;
    QString getYfinServer(void) const noexcept;
    int getYfinPort(void) const noexcept;
    bool read(void) noexcept;
    bool validate(const QJsonObject& root) noexcept;
    void print(void) const noexcept;

private:
    inline static Config* instance_ = nullptr;
    dbConn dbConfig;
    yfinanceConn yfinConfig;
    Config();
    





};

#endif