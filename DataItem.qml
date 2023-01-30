import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14

Rectangle
{
    id: itemRec

    property var title: ""
    property var value: ""
    property var backgroundColor: "#00000000"
    property var colorValue: ""

    radius: 10
    color:  backgroundColor

    Text{
        id: titleTxt
        text: title
        font.pixelSize: 12
        color: "black"
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text{
        id: valueTxt
        text: value

        font.pixelSize: 15
        color: colorValue
        anchors.top: titleTxt.top
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
    }
}