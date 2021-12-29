import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components.material 1.0

Window
{
    id:newDeppsitWindow

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
            popup.onClosed: console.log("Combo Cliked " + coinsModel.get(currentIndex))

        }

        ComboBox
        {
            id: exchangeCBox
            model: ["Binance", "B2M"]
        }

        Button
        {
            id: acceptBtnNewWaa
            text: "Accept"
            onClicked: {
                if(brokerManager.addWallet(coins.currentText.split(" ")[0], exchangeCBox.currentText) == false)
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
        width: 200
        model: walletsModelDeposit
        textRole: "display"
        popup.onClosed: console.log("Combo Cliked " + walletsModelDeposit.getWalletID(currentIndex))
    }

    MaterialTextInput
    {
        id: pair1Amount
        anchors.top: wallet.bottom
        anchors.topMargin: 10
        anchors.left: wallet.left
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
            brokerManager.newDeposit(1,pair1Amount.text_, feesAmount.text_, "", "") //TODO: comments
            newDeppsitWindow.close()
        }
    }

}