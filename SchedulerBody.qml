import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material
import es.broker 1.0
import Astral 1.0

ListView{
    id: schedulerBody

    Component.onCompleted:{
        console.log(operationsModel.rowCount())
    }

    Button
    {
        id: newOperation
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 20
        text: "New Operation"
        onClicked: {
            formsLoader.source = "NewOperationForm.qml"
        }
    }

    Button
    {
        id: newTransfer
        anchors.top: newOperation.top
        anchors.topMargin: 0
        anchors.left: newOperation.right
        anchors.leftMargin: 20
        text: "New Transfer"
        onClicked: {
            formsLoader.source = "NewTransferForm.qml"
        }
    }

    Button
    {
        id: importOperation
        anchors.top: newTransfer.top
        anchors.topMargin: 0
        anchors.left: newTransfer.right
        anchors.leftMargin: 20
        text: "Import"
        onClicked: {
            formsLoader.source = "ImportOperationForm.qml"
        }
    }


    ListView{
        id: favs
        maximumFlickVelocity: 1200.0
        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        orientation: Qt.Vertical
        spacing: 10
        clip: true
        model: operationsModel
        delegate: Operation{}
    }

    ListView{
        id: schedules
        clip: true
    }



}
