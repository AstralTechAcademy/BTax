import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14
import es.broker.components.material 1.0

Window
{
    id:newDepositWindow

    visible: true
    height: 640
    width: 800
    //visibility: "FullScreen"
    color: "#e9e7e7"
    title: qsTr("New Operation")

    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2

    Component.onCompleted: {
        console.log("New Deposit Form")
    }

    signal close()

    Button
    {
        id: newWalletBtn
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
        text: "New Wallet"
        onClicked: {
            newWalletsCombox.visible = true
        }
    }

    Row
    {
        id: newWalletsCombox
        anchors.top: newWalletBtn.top
        anchors.topMargin: 0
        anchors.left: newWalletBtn.right
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 10
        visible: false
        spacing: 10

        ComboBox
        {
            id: coins
            model: coinsModel
            textRole: "name"
            popup.onClosed: console.log("Combo Cliked " + coins.currentText)

        }

        ComboBox
        {
            id: exchangeCBox
            textRole: "name"
            model: exchangesModel //["Binance", "B2M", "Coinbase", "Crypto", "Solflare", "Phantom"]
            popup.onClosed: console.log("Combo Cliked " + exchangeCBox.currentText)
        }

        Button
        {
            id: acceptBtnNewWaa
            text: "Accept"
            onClicked: {
                console.log(coins.currentText  + " "  +exchangeCBox.currentText) 
                if(brokerManager.addWallet(coins.currentText.split(" ")[1], exchangeCBox.currentText.split(" ")[1]) == false)
                    console.log("Error al crear la wallet")
                newWalletsCombox.visible = false
            }
        }
    }

    Text
    {
        id: walletTxt
        anchors.top: parent.top
        anchors.topMargin: 120
        anchors.left: parent.left
        anchors.leftMargin: 50
        text: "Wallet"
    }

    ComboBox
    {
        id: wallet
        anchors.top: walletTxt.bottom
        anchors.topMargin: 10
        anchors.left: walletTxt.left
        anchors.leftMargin: 0
        width: 250
        model: walletsModelAll
        textRole: "display"
        popup.onClosed: console.log("Combo Cliked " + walletsModelAll.getWalletID(currentIndex))
    }

    Text
    {
        id: pair1AmountTxt
        anchors.top: wallet.bottom
        anchors.topMargin: 10
        anchors.left: wallet.left
        anchors.leftMargin: 0
        text: "Amount (discounting fees)"
    }

    MaterialTextInput
    {
        id: pair1Amount
        anchors.top: pair1AmountTxt.bottom
        anchors.topMargin: 10
        anchors.left: pair1AmountTxt.left
        anchors.leftMargin: 0
        text_: "Amount"
    }

//////////Fees

    Text
    {
        id: feesTxt
        anchors.top: pair1Amount.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 50
        text: "Fees"
    }

    MaterialTextInput
    {
        id: feesAmount
        anchors.top: feesTxt.bottom
        anchors.topMargin: 10
        anchors.left: feesTxt.left
        anchors.leftMargin: 0
        text_: "Fees Amount"
    }

/////// Bottom

    Button
    {
        id: accept
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Accept"
        onClicked: {
            brokerManager.newDeposit(walletsModelAll.getWalletID(wallet.currentIndex),pair1Amount.text_, feesAmount.text_, "", "") //TODO: comments
            newDepositWindow.close()
        }
    }

}