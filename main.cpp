#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>
#include "Broker.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    qmlRegisterType<Broker>("es.broker", 1, 0, "BrokerImpl");
    //qmlRegisterType(QUrl("qrc:Broker.qml"), "es.broker", 1, 0, "Broker");
    qmlRegisterType(QUrl("qrc:MaterialTextInput.qml"), "es.broker.components.material", 1, 0, "TextInput");
    qmlRegisterType(QUrl("qrc:Operation.qml"), "es.broker", 1, 0, "Operation");


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    engine.load(url);



    return app.exec();
}
