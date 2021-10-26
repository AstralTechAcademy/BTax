#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Broker.h"
#include "OperationsModel.h"


int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    qmlRegisterType<Broker>("es.broker", 1, 0, "BrokerImpl");
    //qmlRegisterType<OperationsModel>("es.broker", 1, 0, "OperationsModel");
    qmlRegisterUncreatableType<OperationsModel>("es.broker", 1, 0, "OperationsModel", "OperationsModel sholud not be created in QMl");
    //qmlRegisterType(QUrl("qrc:Broker.qml"), "es.broker", 1, 0, "Broker");
    qmlRegisterType(QUrl("qrc:MaterialTextInput.qml"), "es.broker.components.material", 1, 0, "TextInput");
    qmlRegisterType(QUrl("qrc:IconButton.qml"), "es.broker.components", 1, 0, "IconButton");
    qmlRegisterType(QUrl("qrc:Operation.qml"), "es.broker", 1, 0, "Operation");
    qmlRegisterType(QUrl("qrc:SchedulerBody.qml"), "es.broker", 1, 0, "Scheduler");

    OperationsModel operationsModel;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.rootContext()->setContextProperty("operationsModel", &operationsModel);
    engine.load(url);



    return app.exec();
}
