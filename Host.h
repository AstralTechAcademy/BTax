#ifndef HOST_H
#define HOST_H
#include <QString>
#include <QDir>
#include <QStandardPaths>




const QString WinBtaxPath = QDir::homePath()+ "/.Btax/";
const QString LinuxBtaxPath = QDir::homePath()+ "/.Btax/";
const QString DarwinBtaxPath = QDir::homePath()+ "/.Btax/";

const QString WinLogPath = WinBtaxPath + "Logs/";
const QString LinuxLogPath = LinuxBtaxPath + "Logs/";
const QString DarwinLogPath = DarwinBtaxPath + "Logs/";

const QString ConfigFile = "btax.json";
const QString WinConfigPath = WinBtaxPath + ConfigFile;
const QString LinuxConfigPath = LinuxBtaxPath + ConfigFile;
const QString DarwinConfigPath = DarwinBtaxPath + ConfigFile;

class Host {

public:
    static Host *getInstance() {
        if (!instance_)
            instance_ = new Host;
        return instance_;
    }

    const QString &getDirPath() const;
    const QString &getConfigPath() const;
    const QString &getLogPath() const;

private:
    Host(void);
    static Host* instance_;
    QString dirPath_;
    QString configPath_;
    QString logPath_;

    void create(void) const noexcept;
    void printPaths(void);
    
};

#endif