import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import Broker 1.0


Window {
    id: mainWindow
    visible: true
    height: Screen.height
    width: Screen.width
    //visibility: "FullScreen"
    color: "#e9e7e7"
    title: qsTr("Broker")

    Item {
        id: logo
        width: 128
        height: 60
        anchors.left: parent.left
        anchors.leftMargin: 39
        anchors.top: parent.top
        anchors.topMargin: 23

        Rectangle {
            id: rectangle2
            color: "#00000000"
            anchors.fill: parent
            border.width: 3
            border.color: "black"

            Image {
                id: image
                anchors.fill: parent
                source: ""
                fillMode: Image.PreserveAspectFit
            }
        }
    }
    

    Item {
        id: body
        anchors.top: logo.bottom
        anchors.topMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
      
        Loader{
            id: bodyLoader
            anchors.fill: parent
            source: "body.qml"
        }

        
    }

}

/*##^##
Designer {
    D{i:1;anchors_height:60;anchors_width:128;anchors_x:39;anchors_y:23}D{i:14;anchors_x:492}
D{i:5;anchors_height:200;anchors_width:200}
}
##^##*/
