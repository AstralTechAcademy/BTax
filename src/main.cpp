#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QLocale>
#include <future>
#include <unistd.h>
#include "Host.h"
#include "Config.h"
#include "Broker.h"
#include "OperationsModel.h"
#include "BrokerManager.h"
#include "YFinance.h"
#include "SQLManager.h"
#include "WalletsModel.h"
#include "WalletsPercModel.h"
#include "UsersModel.h"
#include "CoinsModel.h"
#include "ExchangesModel.h"
#include "AssetTypeModel.h"
#include "IfExchanges/Importer.h"
#include "IMarketData/Coingecko.h"
#include "Notifications/NotificationManager.h"

#include "logger.h"

void timeout(void)
{
    sleep(5);
    std::cout << "TIMEOUT" << std::endl;
}


int main(int argc, char *argv[])
{
    QString version = "2.02.000";
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickStyle::setStyle("Imagine");
    
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    qmlRegisterUncreatableType<Broker>("es.broker", 1, 0, "Broker", "App Class Instance");
    qmlRegisterUncreatableType<UsersModel>("es.broker", 1, 0, "UsersModel", "Se crea despues");
    //qmlRegisterType<OperationsModel>("es.broker", 1, 0, "OperationsModel");
    qmlRegisterUncreatableType<OperationsModel>("es.broker", 1, 0, "OperationsModel", "OperationsModel sholud not be created in QMl");
    qmlRegisterUncreatableType<Coin>("es.broker", 1, 0, "Coin", "OperationsModel sholud not be created in QMl");
    qmlRegisterUncreatableType<CoinsModel>("es.broker", 1, 0, "CoinsModel", "OperationsModel sholud not be created in QMl");
    qmlRegisterUncreatableType<AssetTypeModel>("es.broker", 1, 0, "AssetTypesModel", "OperationsModel sholud not be created in QMl");
    qmlRegisterUncreatableType<Operation>("es.broker", 1, 0, "Operation", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<WalletsModel>("es.broker", 1, 0, "WalletsModel", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<WalletsModel>("es.broker", 1, 0, "WalletsPercModel", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<BrokerManager>("es.broker", 1, 0, "BrokerManager", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<NotificationManager>("es.notifications", 1, 0, "NotificationManager", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<Importer>("es.broker", 1, 0, "Importer", "Importer sholud not be created in QMl");
    //qmlRegisterType(QUrl("qrc:Broker.qml"), "es.broker", 1, 0, "Broker");
    qmlRegisterType(QUrl("qrc:MaterialTextInput"), "es.broker.components.material", 1, 0, "MaterialTextInput");
    qmlRegisterType(QUrl("qrc:IconButton"), "es.broker.components", 1, 0, "IconButton");
    qmlRegisterType(QUrl("qrc:Operation"), "es.broker", 1, 0, "Operation");
    qmlRegisterType(QUrl("qrc:MainWindow"), "es.broker", 1, 0, "Main");
    qmlRegisterType(QUrl("qrc:SchedulerBody"), "es.broker", 1, 0, "Scheduler");
    qmlRegisterType(QUrl("qrc:WalletsBody"), "es.broker", 1, 0, "WalletsBody");
    qmlRegisterType(QUrl("qrc:Wallet"), "es.broker", 1, 0, "Wallet");
    qmlRegisterType(QUrl("qrc:NewOperationForm"), "es.broker", 1, 0, "NewOperationForm");
    qmlRegisterType(QUrl("qrc:NewTransferForm"), "es.broker", 1, 0, "NewTransferForm");
    qmlRegisterType(QUrl("qrc:NewDepositForm"), "es.broker", 1, 0, "NewDepositForm");
    qmlRegisterType(QUrl("qrc:NewAssetForm"), "es.broker", 1, 0, "NewAssetForm");
    qmlRegisterType(QUrl("qrc:ImportOperationForm"), "es.broker", 1, 0, "ImportOperationForm");
    qmlRegisterType(QUrl("qrc:DateSelector"), "es.broker.components", 1, 0, "DateSelector");
    qmlRegisterType(QUrl("qrc:DataItem"), "es.broker.components", 1, 0, "Data");
    qmlRegisterType(QUrl("qrc:AMessage"), "es.notifications", 1, 0, "AMessage");
    qmlRegisterType(QUrl("qrc:AMessageDialog"), "es.notifications", 1, 0, "AMessageDialog");

    QQmlApplicationEngine engine;

    Broker* broker = Broker::getInstance(version);
    engine.rootContext()->setContextProperty("BrokerImpl", broker);
    const QUrl url(QStringLiteral("qrc:/InitWindow"));

    UsersModel usersModel;
    OperationsModel operationsModel;
    WalletsModel walletsModel;
    WalletsModel walletsModelAll;
    WalletsPercModel walletsPercModel;
    CoinsModel coinsModel;
    ExchangesModel exchangesModel;
    AssetTypeModel assetTypeModel;
    NotificationManager* notificationManager = NotificationManager::getInstance();

    engine.rootContext()->setContextProperty("operationsModel", &operationsModel);
    engine.rootContext()->setContextProperty("walletsModel", &walletsModel);
    engine.rootContext()->setContextProperty("walletsModelAll", &walletsModelAll);
    engine.rootContext()->setContextProperty("walletsPercModel", &walletsPercModel);
    engine.rootContext()->setContextProperty("usersModel", &usersModel);
    engine.rootContext()->setContextProperty("coinsModel", &coinsModel);
    engine.rootContext()->setContextProperty("exchangesModel", &exchangesModel);
    engine.rootContext()->setContextProperty("assetTypesModel", &assetTypeModel);
    engine.rootContext()->setContextProperty("notificationManager", notificationManager);

    BrokerManager* brokerManager = BrokerManager::getInstance(&operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel, &assetTypeModel, &exchangesModel);
    engine.rootContext()->setContextProperty("brokerManager", brokerManager);

    Importer *importer = Importer::getInstance(std::shared_ptr<BrokerManager>(brokerManager));
    engine.rootContext()->setContextProperty("importer", importer);

    engine.load(url);
    return app.exec();
}
