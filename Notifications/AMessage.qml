import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14

Rectangle
{
    id: messageBorder
    radius: 10
    color: "#00000000" //"lightgrey"

    property var message: ""
    property var icon: ""

    Text{
        id: errorMsg
        text: message
        font.pixelSize: 12
        color: "black"
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
    }
}