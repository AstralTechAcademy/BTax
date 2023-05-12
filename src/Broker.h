//
// Created by gabridc on 26/8/21.
//

#ifndef BROKER_BROKER_H
#define BROKER_BROKER_H
#include <QObject>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QVariant>
#include "OperationsModel.h"
#include "BrokerManager.h"
#include "Config.h"
#include "SQLManager.h"
#include "WalletsModel.h"
#include "WalletsPercModel.h"
#include "UsersModel.h"
#include "CoinsModel.h"
#include "AssetTypeModel.h"
#include "IfExchanges/Importer.h"
#include "IMarketData/Coingecko.h"
#include "Notifications/NotificationManager.h"

class Broker : public QObject
{
    Q_OBJECT

public:
    Broker(const QString& version, QObject *parent = nullptr);


    static Broker* getInstance(const QString& version) {
        if (!instance)
            instance = new Broker(version);
        return instance;
    }


    static Broker* getInstance(void) {
        return instance;
    }

    enum class HOSTS{
        WIN = 0,
        MACOSX = 1,
        LINUX = 2,
        ANDROIDD = 3,
        IOS = 4
    };

signals:
    void notOpened(void);
    void connectingDatabase(void);
    void opened(void);
    void loaded(void);
    void notLoaded(void);
    void noUsers(void);
    void userAlready(void);
    void loading(void);
    void configNotValid(void);

public slots:
    QString getServer(void) const;
    QString getVersion(void) const;
    QString getDatabase(void) const;
    bool isOpened(void) const;
    bool openDatabase(void);
    #ifdef GTEST
    BrokerManager::LoadResCode load(void);
    #else
    void load(void);
    #endif
    void newUser(const QString& name);
    void close(void) const noexcept { qApp->exit(0);}



private:
    static QString server_;
    static QString version_;
    inline static Config* config_ = nullptr;
    inline static QString database_;
    static bool dbOpened_;

    inline static Broker *instance;

};


#endif //BROKER_BROKER_H
