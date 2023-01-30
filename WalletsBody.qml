import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
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
        id: cryptoInvested
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: walletsBody.left
        anchors.leftMargin: 40
        width: 80
        visible: true
        title: "Crypto Invested"
        value: parseFloat( walletsModel.getCryptoInvested().toFixed(6) ) + " " + "EUR"
        colorValue: "black"
    }

    Components.Data
    {
        id: totalInvested
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: cryptoInvested.right
        anchors.leftMargin: 60
        width: 80
        visible: true
        title: "Deposited"
        value: parseFloat( walletsModel.getTotalInvestedFiat().toFixed(6) ) + " " + "EUR"
        colorValue: "black"
    }

    Components.Data
    {
        id: totalEarnings
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: totalInvested.right
        anchors.leftMargin: 60
        width: 80
        visible: true
        title: "Total Earnings"
        value: parseFloat( operationsModel.totalEarnings.toFixed(6) ) + " " + "EUR"
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

    Button
    {
        id: newAsset
        anchors.top: totalInvested.top
        anchors.topMargin: 0
        anchors.right: newDeposit.left
        anchors.rightMargin: 20
        text: "New Asset"
        onClicked: {
            formsLoader.source = "NewAssetForm.qml"
        }
    }

    Item
    {
        id: percentageBar
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: totalInvested.bottom
        anchors.topMargin: 50
        height: 20

        Row
        {
            Repeater
            {
                id: repeaterPerc
                model: walletsPercModel
                delegate: Rectangle {
                    id: percentageItem
                    height: percentageBar.height - 5
                    width: portfolioPercentage * percentageBar.width / 100
                    color: coinColor

                    Text
                    {
                        id: nameCoin
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.bottom
                        anchors.topMargin: 3
                        text: coin
                        visible: true
                    }
                }
            }
        }
    }


ScrollView
{
    anchors.top: percentageBar.bottom
    anchors.topMargin: 10
    anchors.left: walletsBody.left
    anchors.leftMargin: 10
    anchors.right: walletsBody.right
    anchors.rightMargin: 10
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 10
    clip: true

    Connections
    {
        target: brokerManager

       function onDepositCompleted() {
            console.log("Deposit done")
       }
    }

    Column
    {
        id: wallets
        anchors.fill: parent

        Repeater
        {
            id: repeater
            Component.onCompleted: console.log("Number Wallets: " + walletsModel.rowCount())
            model: walletsModel
            delegate: Wallet{
                height: 70
                width: wallets.width
            }
        }
    }
}




}