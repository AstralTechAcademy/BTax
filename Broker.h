//
// Created by gabridc on 26/8/21.
//

#ifndef BROKER_BROKER_H
#define BROKER_BROKER_H
#include <QObject>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QVariant>
#include <QDebug>
#include "OperationsModel.h"
#include "BrokerManager.h"
#include "DBLocal.h"
#include "DBRemote.h"
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
    Broker(const QString& server, const QString& version, QQmlApplicationEngine& engine, QObject *parent = nullptr);


    static Broker* getInstance(const QString& server, const QString& version, QQmlApplicationEngine& engine) {
        if (!instance)
            instance = new Broker(server, version, engine);
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
    void loading(void);

public slots:
    int getHost(void) const;
    QString getServer(void) const;
    QString getVersion(void) const;
    bool isOpened(void) const;
    void openDatabase(void);

    void load(void);




private:
    static HOSTS host_;
    static QString server_;
    static QString version_;
    static bool dbOpened_;

    inline static Broker *instance;

    QQmlApplicationEngine& engine_;

};


#endif //BROKER_BROKER_H
