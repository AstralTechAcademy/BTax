#ifndef CONFIG_H
#define CONFIG_H
#include "Host.h"
#include <QString>

class Config
{

public:
    static Config* getInstance(void)
    {
        if(instance_ == nullptr)
            return new Config();
        return instance_;
    }

    QString getServer(void) const noexcept;
    int getPort(void) const noexcept;
    QString getDatabasename(void) const noexcept;
    QString getUsername(void) const noexcept; 
    QString getPassword(void) const noexcept;
    bool read(void) noexcept;

private:
    inline static Config* instance_ = nullptr;
    QString server_ = "";
    QString databasename_ = "";
    QString username_ = "";
    QString password_ = "";
    int port_ = 0;
    Config();
    





};

#endif