#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Broker.h"
#include "OperationsModel.h"
#include "BrokerManager.h"
#include "DBLocal.h"
#include "WalletsModel.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    qmlRegisterType<Broker>("es.broker", 1, 0, "BrokerImpl");
    //qmlRegisterType<OperationsModel>("es.broker", 1, 0, "OperationsModel");
    qmlRegisterUncreatableType<OperationsModel>("es.broker", 1, 0, "OperationsModel", "OperationsModel sholud not be created in QMl");
    qmlRegisterUncreatableType<Operation>("es.broker", 1, 0, "Operation", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<WalletsModel>("es.broker", 1, 0, "WalletsModel", "Operation sholud not be created in QMl");
    qmlRegisterUncreatableType<BrokerManager>("es.broker", 1, 0, "BrokerManager", "Operation sholud not be created in QMl");
    //qmlRegisterType(QUrl("qrc:Broker.qml"), "es.broker", 1, 0, "Broker");
    qmlRegisterType(QUrl("qrc:MaterialTextInput.qml"), "es.broker.components.material", 1, 0, "TextInput");
    qmlRegisterType(QUrl("qrc:IconButton.qml"), "es.broker.components", 1, 0, "IconButton");
    qmlRegisterType(QUrl("qrc:Operation.qml"), "es.broker", 1, 0, "Operation");
    qmlRegisterType(QUrl("qrc:SchedulerBody.qml"), "es.broker", 1, 0, "Scheduler");
    qmlRegisterType(QUrl("qrc:WalletsBody.qml"), "es.broker", 1, 0, "WalletsBody");
    qmlRegisterType(QUrl("qrc:Wallet.qml"), "es.broker", 1, 0, "Wallet");
    qmlRegisterType(QUrl("qrc:NewOperationForm.qml"), "es.broker", 1, 0, "NewOperationForm");
    qmlRegisterType(QUrl("qrc:NewDepositForm.qml"), "es.broker", 1, 0, "NewDepositForm");
    qmlRegisterType(QUrl("qrc:DateSelector.qml"), "es.broker.components", 1, 0, "DateSelector");
    qmlRegisterType(QUrl("qrc:DataItem.qml"), "es.broker.components", 1, 0, "Data");

    OperationsModel operationsModel;
    WalletsModel walletsModel;



    DBLocal::GetInstance()->createDatabase();
    DBLocal::GetInstance()->openDatabase();
    BrokerManager* brokerManager = new BrokerManager(0, &operationsModel, &walletsModel);
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
    engine.rootContext()->setContextProperty("brokerManager", brokerManager);
    engine.load(url);



    return app.exec();
}
