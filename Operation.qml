import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material


Rectangle{

    color: "black"
    radius: 10
    width: 200
    height: 100

    property alias coinName_: coinName.text
    property alias deposit_: deposit.text
    property alias retired_: retired.text


    Text{
        id: coinName
        color: "white"
        font.pixelSize: 15
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
    }

    Text{
        id: deposit
        color: "red"
        font.pixelSize: 15
        anchors.top: coinName.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10
    }

    Text{
        id: retired
        color: "green"
        font.pixelSize: 15
        anchors.top: deposit.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10
    }



}