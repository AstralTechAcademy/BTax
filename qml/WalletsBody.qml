import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material
import es.broker.components 1.0 as Components
import es.broker 1.0
import es.filters 1.0
import Astral 1.0

Item
{

    id: walletsBody
    anchors.fill: parent

    property bool walletsFound_: false

    Text
    {
        id: portfolio
        text: "Portfolio"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: walletsBody.left
        anchors.leftMargin: 20
        font.bold: true
        font.pixelSize : 30
        opacity: 0.5

    }

    Components.Data
    {
        id: noNcryptoInvested
        anchors.top: portfolio.bottom
        anchors.topMargin: 20
        anchors.left: walletsBody.left
        anchors.leftMargin: 50
        width: 80
        visible: true
        title: "Non Crypto Invested"
        value: parseFloat( walletsModel.getNonCryptoInvested().toFixed(6) ) + " " + "EUR"
        colorValue: "black"
    }    

    Components.Data
    {
        id: cryptoInvested
        anchors.top: noNcryptoInvested.top
        anchors.topMargin: 0
        anchors.left: noNcryptoInvested.right
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
        anchors.top: noNcryptoInvested.top
        anchors.topMargin: 0
        anchors.left: cryptoInvested.right
        anchors.leftMargin: 60
        width: 80
        visible: true
        title: "Total Invested"
        value: parseFloat( walletsModel.getTotalInvested2().toFixed(6) ) + " " + "EUR"
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
            formsLoader.source = "NewDepositForm"
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
            formsLoader.source = "NewAssetForm"
        }
    }

    Item
    {
        id: percentageBar
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 30
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

                    Component.onCompleted: console.log("PortPerc: " + portfolioPercentage + " " + percentageBar.width)

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


/*ListView {

    id: tabs
    anchors.top: percentageBar.bottom
    anchors.topMargin: 5
    anchors.left: walletsBody.left
    anchors.leftMargin: 10
    anchors.right: walletsBody.right
    anchors.rightMargin: 10

    width: 210
    height: 100

    orientation: Qt.Horizontal
    spacing: 5

    model: ListModel {
        ListElement {
            name: "Shares & Funds"
        }
        ListElement {
            name: "Crypto"
        }
    }

    delegate: ATabButton
    {
        onClicked: {
            walletsFilterManager.filter("AssetType", name);
        }
    }
}*/

Row
{
    id: tabs
    anchors.top: percentageBar.bottom
    anchors.topMargin: 30
    anchors.left: walletsBody.left
    anchors.leftMargin: 30
    anchors.right: walletsBody.right
    anchors.rightMargin: 10
    spacing: 5
    z:2

    height: 100
    width: 210

    ComboBox
    {
        id: assetCategory
        model: ["Traditional" , "Crypto"]
        popup.onClosed: {
            walletsFilterManager.filter("AssetCategory", model[currentIndex]);
        }
    }

    ComboBox
    {
        id: assetType
        model: assetTypesModel
        textRole: "name"
        popup.onClosed: {
            walletsFilterManager.filter("AssetType", assetTypesModel.getName(currentIndex));
        }
    }    

    ASelectionCombo
    {
        id: assetType1
        z:2
        model: assetTypesModel
        onClicked: name => walletsFilterManager.filter("AssetType", name);
        onRemoved: name => walletsFilterManager.remove("AssetType", name);
    }

    Button
    {
        text: "Apply"
        onClicked: {
            walletsFilterManager.apply();
        }
    }

    Button
    {
        text: "Clean"
        onClicked: {
            walletsFilterManager.clean();
        }
    }
}

Text
{
    anchors.top: tabs.bottom
    anchors.topMargin: 10
    anchors.horizontalCenter: walletsBody.horizontalCenter
    visible: walletsFound_
    text: "No wallets found"
}

ScrollView
{
    visible: walletsFound_
    anchors.top: tabs.bottom
    anchors.topMargin: 10
    anchors.left: percentageBar.left
    anchors.leftMargin: 0
    anchors.right: percentageBar.right
    anchors.rightMargin: 0
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 10
    clip: true
    z:1

    Column
    {
        id: wallets
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5

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

Connections
{
    target: brokerManager

    function onDepositCompleted() {
        console.log("Deposit done")
    }

    function onNoWalletsFound()
    {
        walletsFound_ = false
    }

    function onWalletsFound()
    {
        walletsFound_ = true
    }
}

}