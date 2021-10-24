import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material
import es.broker 1.0

ListView{
    id: schedulerBody

    Component.onCompleted:{
        console.log(operationsModel.rowCount())
    }


    ListView{
        id: favs
        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        orientation: Qt.Horizontal
        spacing: 10
        clip: true
        model: operationsModel
        delegate: Operation{
            id_: model.id
            buyPrice_: model.buyPrice
            //sellPrice: model.sellPrice
            //buyPriceDate: model.buyPriceDate
            //sellPriceDate: model.sellPriceDate
            deposit_: model.deposit
            retired_: model.retired
            status_: model.status
        }
    }

    ListView{
        id: schedules
        clip: true
    }



}
