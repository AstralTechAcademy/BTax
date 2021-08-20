#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    qmlRegisterType(QUrl("qrc:Broker.qml"), "es.broker", 1, 0, "Broker");
    qmlRegisterType(QUrl("qrc:MaterialTextInput.qml"), "es.broker.components.material", 1, 0, "TextInput");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    engine.load(url);



    return app.exec();
}
