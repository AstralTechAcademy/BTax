import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker 1.0



Window {
    id: mainWindow
    visible: true
    height: Screen.height
    width: Screen.width
    //visibility: "FullScreen"
    color: "#e9e7e7"
    title: qsTr("Broker")

    BrokerImpl{
        id: broker
    }


    //OperationsModel{id: operationsModel}

    Component.onCompleted: {
        console.log(broker.getHost())
        bodyLoader.source = "WalletsBody.qml"
    }

    readonly property int host_os: broker.getHost()

    Item {
        id: logo
        width: 128
        height: 60
        anchors.horizontalCenter: parent.horizontalCenter
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
        anchors.bottom: menuBar.top
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
      
        Loader{
            id: bodyLoader
            anchors.fill: parent

        }

        
    }

    Rectangle{
        id: menuBar
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        height: 35
        color: "gray"

        Image{
            id: homeButton
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:assets/home"
            height: 23
            width: 23
            MouseArea{
                id: homeMA
                anchors.fill: parent
                onClicked: bodyLoader.source = "body.qml"
            }
        }

        Image{
            id: averageButton
            anchors.left: homeButton.right
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:assets/average-white"
            height: 30
            width: 30
            MouseArea{
                id: avgMA
                anchors.fill: parent
                onClicked: bodyLoader.source = ""
            }
        }


        Image{
            id: scheduleButton
            anchors.right: homeButton.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:assets/schedule-white"
            height: 30
            width: 30
            MouseArea{
                id: schMA
                anchors.fill: parent
                onClicked: bodyLoader.source = "SchedulerBody.qml"
            }
        }

        Image{
            id: walletsButton
            anchors.right: scheduleButton.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:assets/wallets-white"
            height: 30
            width: 30
            MouseArea{
                id: walletMA
                anchors.fill: parent
                onClicked: bodyLoader.source = "WalletsBody.qml"
            }
        }


    }

}

/*##^##
Designer {
    D{i:1;anchors_height:60;anchors_width:128;anchors_x:39;anchors_y:23}D{i:14;anchors_x:492}
D{i:5;anchors_height:200;anchors_width:200}
}
##^##*/
