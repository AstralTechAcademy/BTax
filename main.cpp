#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Broker.h"
#include "OperationsModel.h"
#include "DBLocal.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    qmlRegisterType<Broker>("es.broker", 1, 0, "BrokerImpl");
    //qmlRegisterType<OperationsModel>("es.broker", 1, 0, "OperationsModel");
    qmlRegisterUncreatableType<OperationsModel>("es.broker", 1, 0, "OperationsModel", "OperationsModel sholud not be created in QMl");
    qmlRegisterUncreatableType<Operation>("es.broker", 1, 0, "Operation", "Operation sholud not be created in QMl");
    //qmlRegisterType(QUrl("qrc:Broker.qml"), "es.broker", 1, 0, "Broker");
    qmlRegisterType(QUrl("qrc:MaterialTextInput.qml"), "es.broker.components.material", 1, 0, "TextInput");
    qmlRegisterType(QUrl("qrc:IconButton.qml"), "es.broker.components", 1, 0, "IconButton");
    qmlRegisterType(QUrl("qrc:Operation.qml"), "es.broker", 1, 0, "Operation");
    qmlRegisterType(QUrl("qrc:SchedulerBody.qml"), "es.broker", 1, 0, "Scheduler");

    OperationsModel operationsModel;

    DBLocal::GetInstance()->createDatabase();
    DBLocal::GetInstance()->openDatabase();
    DBLocal::GetInstance()->getWallets("Gabriel");
    auto operations = DBLocal::GetInstance()->getOperations();

    double gananciasAnuales = 0.0;
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

    std::cout << "Depositado B2M: " << invertido << std::endl;

    if(std::get<0>(operations) == true)
    {
        for(auto op : std::get<1>(operations))
            operationsModel.add(op);
    }

    invertido = DBLocal::GetInstance()->getInvested("Gabriel", "B2M", "B2M");

    std::cout << "Invertido B2M: " <<  invertido << std::endl;

    invertido = DBLocal::GetInstance()->getInvested("Gabriel", "Binance", "ADA");

    std::cout << "Invertido ADA: " <<  invertido << std::endl;

    invertido = DBLocal::GetInstance()->getInvested("Gabriel", "Binance", "DOT");

    std::cout << "Invertido DOT: " <<  invertido << std::endl;

    invertido = DBLocal::GetInstance()->getInvested("Gabriel", "Binance", "MATIC");

    std::cout << "Invertido MATIC: " <<  invertido << std::endl;

    invertido = DBLocal::GetInstance()->getInvested("Gabriel", "Binance", "SOL");

    std::cout << "Invertido SOL: " <<  invertido << std::endl;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.rootContext()->setContextProperty("operationsModel", &operationsModel);
    engine.load(url);



    return app.exec();
}
