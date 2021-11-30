import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material
import es.broker.components 1.0 as Components
import es.broker 1.0

Item
{

    id: walletsBody
    anchors.fill: parent


    Components.Data
    {
        id: totalInvested
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: walletsBody.left
        anchors.leftMargin: 40
        width: 80
        visible: true
        title: "Total Invested"
        value: parseFloat( walletsModel.getTotalInvested().toFixed(6) ) + " " + "EUR"
        colorValue: "black"
    }


    Button
    {
        id: newDeposit
        anchors.top: totalInvested.top
        anchors.topMargin: 0
        anchors.right: walletsBody.right
        anchors.rightMargin: 20
        text: "New Deposit"
        onClicked: {
            formsLoader.source = "NewDepositForm.qml"
        }
    }


ScrollView
{
            anchors.top: totalInvested.bottom
            anchors.topMargin: 50
            anchors.left: walletsBody.left
            anchors.leftMargin: 10
            anchors.right: walletsBody.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            clip: true
    Column
    {
        id: wallets
        anchors.fill: parent


        Repeater
        {
            id: repeater
            Component.onCompleted: console.log(walletsModel.rowCount())
            model: walletsModel
            delegate: Wallet{

                height: 70
                width: wallets.width
            }
        }


    }
}




}