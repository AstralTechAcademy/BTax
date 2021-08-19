import sys

sys.path.append('D:\Qt-Workspace\Asteria\PySide2')
from PySide2.QtQml import *
from PySide2.QtGui import QGuiApplication
from PySide2.QtQuick import QQuickView
from PySide2.QtCore import QUrl, QObject, Signal, Slot, Property
from Broker import *

def main():
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()

    qmlRegisterType(Broker, 'Broker', 1, 0, 'Broker')

    mainWindow = Broker()

    engine.load(QUrl("main.qml"))
  
    if not engine.rootObjects():
        sys.exit(-1)    

    app.exec_()


if __name__ == "__main__":
    main()




   

