from PySide2.QtQml import *
from PySide2.QtGui import QGuiApplication
from PySide2.QtQuick import QQuickView
from PySide2.QtCore import QUrl, QObject, Signal, Slot, Property

class Broker(QObject):
    def __init__(self):
        QObject.__init__(self)