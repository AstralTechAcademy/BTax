#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <future>
#include <unistd.h>
#include "Broker.h"
#include "OperationsModel.h"
#include "BrokerManager.h"
#include "DBLocal.h"
#include "DBRemote.h"
#include "WalletsModel.h"
#include "WalletsPercModel.h"
#include "UsersModel.h"
#include "CoinsModel.h"
#include "IfExchanges/Importer.h"

void timeout(void)
{
    sleep(5);
    std::cout << "TIMEOUT" << std::endl;
}


int main(int argc, char *argv[])
{
    QString version = "1.0.1";
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);



    QApplication app(argc, argv);
    qmlRegisterUncreatableType<Broker>("es.broker", 1, 0, "Broker", "App Class Instance");
    qmlRegisterUncreatableType<UsersModel>("es.broker", 1, 0, "UsersModel", "Se crea despues");
    //qmlRegisterType<OperationsModel>("es.broker", 1, 0, "OperationsModel");
    qmlRegisterUncreatableType<OperationsModel>("es.broker", 1, 0, "OperationsModel", "OperationsModel sholud not be created in QMl");
    qmlRegisterUncreatableType<Coin>("es.broker", 1, 0, "Coin", "OperationsModel sholud not be created in QMl");
    qmlRegisterUncreatableType<CoinsModel>("es.broker", 1, 0, "CoinsModel", "OperationsModel sholud not be created in QMl");
    qmlRegisterUncreatableType<Operation>("es.broker", 1, 0, "Operation", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<WalletsModel>("es.broker", 1, 0, "WalletsModel", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<WalletsModel>("es.broker", 1, 0, "WalletsPercModel", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<BrokerManager>("es.broker", 1, 0, "BrokerManager", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<Importer>("es.broker", 1, 0, "Importer", "Importer sholud not be created in QMl");
    //qmlRegisterType(QUrl("qrc:Broker.qml"), "es.broker", 1, 0, "Broker");
    qmlRegisterType(QUrl("qrc:MaterialTextInput.qml"), "es.broker.components.material", 1, 0, "TextInput");
    qmlRegisterType(QUrl("qrc:IconButton.qml"), "es.broker.components", 1, 0, "IconButton");
    qmlRegisterType(QUrl("qrc:Operation.qml"), "es.broker", 1, 0, "Operation");
    qmlRegisterType(QUrl("qrc:SchedulerBody.qml"), "es.broker", 1, 0, "Scheduler");
    qmlRegisterType(QUrl("qrc:WalletsBody.qml"), "es.broker", 1, 0, "WalletsBody");
    qmlRegisterType(QUrl("qrc:Wallet.qml"), "es.broker", 1, 0, "Wallet");
    qmlRegisterType(QUrl("qrc:NewOperationForm.qml"), "es.broker", 1, 0, "NewOperationForm");
    qmlRegisterType(QUrl("qrc:NewDepositForm.qml"), "es.broker", 1, 0, "NewDepositForm");
    qmlRegisterType(QUrl("qrc:ImportOperationForm.qml"), "es.broker", 1, 0, "ImportOperationForm");
    qmlRegisterType(QUrl("qrc:DateSelector.qml"), "es.broker.components", 1, 0, "DateSelector");
    qmlRegisterType(QUrl("qrc:DataItem.qml"), "es.broker.components", 1, 0, "Data");


    UsersModel usersModel;
    OperationsModel operationsModel;
    WalletsModel walletsModel;
    WalletsModel walletsModelAll;
    WalletsPercModel walletsPercModel;
    CoinsModel coinsModel;


    //DBRemote::GetInstance()->createDatabase();
    /*auto future = std::async(std::launch::async, [](){
        sleep(5);
        std::cout << "TIMEOUT" << std::endl;
    });*/
    auto dbOpened = DBRemote::GetInstance()->openDatabase();
    std:: cout << "DB Server Opened: " <<  dbOpened<< std::endl;

    //DBLocal::GetInstance()->createDatabase();
    //DBLocal::GetInstance()->openDatabase();
    usersModel.setUsers();

    /*double gananciasAnuales = 0.0;
    for(auto operation : std::get<1>(DBLocal::GetInstance()->getOperations("B2M")) )
    {
        gananciasAnuales += operation->getGanancia();
    }

    double invertido = 0.0;
    auto deposits = std::get<1>(DBLocal::GetInstance()->getDeposits("Gabriel"));
    for(auto deposit : deposits)
    {
        std::cout << deposit->getUser().toStdString() << std::endl;
        invertido += deposit->getAmount();
    }

    std::cout << "Invertido: " << invertido << std::endl;
    std::cout << "Ganancias Anuales: " << gananciasAnuales << std::endl;

    deposits = std::get<1>(DBLocal::GetInstance()->getDeposits("Gabriel", "B2M"));
    invertido = 0.0;
    for(auto deposit : deposits)
    {
        std::cout << deposit->getUser().toStdString() << std::endl;
        invertido += deposit->getAmount();
    }

    std::cout << "Depositado B2M: " << invertido << std::endl;*/






    /*invertido = DBLocal::GetInstance()->getInvested("Gabriel", "Binance", "ADA");

    std::cout << "Invertido ADA: " <<  invertido << std::endl;

    invertido = DBLocal::GetInstance()->getInvested("Gabriel", "Binance", "DOT");

    std::cout << "Invertido DOT: " <<  invertido << std::endl;

    invertido = DBLocal::GetInstance()->getInvested("Gabriel", "Binance", "MATIC");

    std::cout << "Invertido MATIC: " <<  invertido << std::endl;

    invertido = DBLocal::GetInstance()->getInvested("Gabriel", "Binance", "SOL");

    std::cout << "Invertido SOL: " <<  invertido << std::endl;*/



    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.rootContext()->setContextProperty("operationsModel", &operationsModel);
    engine.rootContext()->setContextProperty("walletsModel", &walletsModel);
    engine.rootContext()->setContextProperty("walletsModelAll", &walletsModelAll);
    engine.rootContext()->setContextProperty("walletsPercModel", &walletsPercModel);
    engine.rootContext()->setContextProperty("usersModel", &usersModel);
    engine.rootContext()->setContextProperty("coinsModel", &coinsModel);

    BrokerManager* brokerManager = new BrokerManager(0, &operationsModel, &walletsModel, &walletsModelAll, &walletsPercModel, &coinsModel);
    engine.rootContext()->setContextProperty("brokerManager", brokerManager);

    Broker* broker = new Broker(DBRemote::GetInstance()->getServer(), version);
    engine.rootContext()->setContextProperty("BrokerImpl", broker);
    engine.load(url);

    Importer *importer = new Importer(std::shared_ptr<BrokerManager>(brokerManager));
    auto r = importer->import("Crypto", "/home/gabridc/Documentos/crypto_transactions_record_20220422_125541.csv");



    return app.exec();
}
