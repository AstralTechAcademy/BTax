//
// Created by gabridc on 18/1/22.
//

#ifndef BTAX_HOST_H
#define BTAX_HOST_H
#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QStringList>


namespace Btax
{

const QString WinNeptunoPath = QDir::homePath()+ "/.Btax/";
const QString LinuxNeptunoPath = QDir::homePath()+ "/.Btax/";
const QString DarwinNeptunoPath = QDir::homePath()+ "/.Btax/";

const QString WinConfigPath = QDir::homePath()+ "/.Btax/btax.config";
const QString LinuxConfigPath = LinuxNeptunoPath + "btax.config";
const QString DarwinConfigPath = QDir::homePath()+ "/.Btax/btax.config";

const QString WinLogPath = QDir::homePath()+ "/.Btax/Logs/";
const QString LinuxLogPath = LinuxNeptunoPath + "Logs/";
const QString DarwinLogPath = QDir::homePath()+ "/.Btax/Logs/";

class Host {

public:
    Host(void);
    static Host *getInstance() {
        if (!instance_)
            instance_ = new Host;
        return instance_;
    }

    const QString &getDirPath() const;
    const QString &getDbPath() const;
    const QString &getConfigPath() const;
    const QString &getLogPath() const;

private:
    static Host* instance_;
    QString dirPath;
    QString configPath;
    QString logPath_;
    bool mobile_;

    void create(void) const noexcept;
    void printPaths(void);
};

}


#endif //BTAX_HOST_H
