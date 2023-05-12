import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14
import Qt5Compat.GraphicalEffects

Rectangle
{
    id: messageBorder
    radius: 10
    width: errorMsg.width + img.width + 30
    height:30
    color: "#B00020"

    property var message: ""
    property var icon: ""

    Image
    {
        id: img
        source: "qrc:assets/error-black"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
        fillMode: Image.PreserveAspectFit
        height: 25

    }

    ColorOverlay {
        anchors.fill: img
        source: img
        color: "white"  // make image like it lays under red glass 
    }

    Text{
        id: errorMsg
        text: message
        font.pixelSize: 12
        color: "white"
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: img.right
        anchors.leftMargin: 5
    }
}