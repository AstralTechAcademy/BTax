import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components 1.0
import es.broker.components.material 1.0

Window
{
    id:newOperationWindow

    visible: true
    height: 700
    width: 800
    //visibility: "FullScreen"
    color: "#e9e7e7"
    title: qsTr("New Operation")

    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2

    Component.onCompleted: console.log("New Operation Form")

    signal close()

    Text
    {
        id: operationTypeTxt
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
        text: "Operation"
    }

    ComboBox
    {
        id: type
        anchors.top: operationTypeTxt.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 50
        model: ["Compra", "Venta", "Staking"]
    }

    /*Text
    {
        id: exchangeTxt
        anchors.top: operationTypeTxt.top
        anchors.topMargin: 0
        anchors.left: exchange.left
        anchors.leftMargin: 0
        text: "Exchange"
    }

    ComboBox
    {
        id: exchange
        anchors.top: type.top
        anchors.topMargin: 0
        anchors.left: type.right
        anchors.leftMargin: 20
        model: ["B2M", "Binance"]
    }*/

/////// Date

    DateSelector
    {
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: p1AmountFiat.right
        anchors.leftMargin: 50
        width: 300
    }




//////////PAIR 1

    Text
    {
        id: pair1Txt
        anchors.top: type.bottom
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 50
        text: "Wallet Origin (Without fees)"
    }

    ComboBox
    {
        id: pair1
        anchors.top: pair1Txt.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 50
        currentIndex: 0
        model: walletsModelDeposit
        textRole: "display"
        popup.onClosed: console.log("Combo Cliked " + walletsModelDeposit.getWalletID(currentIndex))
    }

    MaterialTextInput
    {
        id: pair1Amount
        anchors.top: pair1.bottom
        anchors.topMargin: 10
        anchors.left: pair1.left
        anchors.leftMargin: 0
        text_: "Amount"

    }

    MaterialTextInput
    {
        id: p1AmountFiat
        text_: "Fiat Price (per coin)"
        anchors.top: pair1Amount.bottom
        anchors.topMargin: 10
        anchors.left: pair1Amount.left
        anchors.leftMargin: 0
    }


//////////PAIR 2

    Text
    {
        id: pair2Txt
        anchors.top: pair1Txt.top
        anchors.topMargin: 0
        anchors.left: p1AmountFiat.right
        anchors.leftMargin: 30
        text: "Wallet Destination (Without fees)"
    }

    ComboBox
    {
        id: pair2
        anchors.top: pair2Txt.bottom
        anchors.topMargin: 10
        anchors.left: pair2Txt.left
        anchors.leftMargin: 0
        model: walletsModelDeposit
        textRole: "display"
        popup.onClosed: console.log("Combo Cliked " + walletsModelDeposit.getWalletID(currentIndex))
    }

    MaterialTextInput
    {
        id: pair2Amount
        anchors.top: pair2.bottom
        anchors.topMargin: 10
        anchors.left: pair2.left
        anchors.leftMargin: 0
        text_: "Amount"
    }

    MaterialTextInput
    {
        id: p2AmountFiat
        text_: "Fiat Price (per coin)"
        anchors.top: pair2Amount.bottom
        anchors.topMargin: 10
        anchors.left: pair2Amount.left
        anchors.leftMargin: 0
    }

//////////Fees

    Text
    {
        id: feesTxt
        anchors.top: p2AmountFiat.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 50
        text: "Fees"
    }

    ComboBox
    {
        id: fees
        anchors.top: feesTxt.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 50
        model: ["ADA", "AVAX", "BNB", "BTC", "ETH", "EUR", "SOL", "USD", "B2M", "DOT", "MATIC", "VET", "IOTA", "CRO"]
        currentIndex: 0

    }

    MaterialTextInput
    {
        id: feesAmount
        anchors.top: fees.bottom
        anchors.topMargin: 10
        anchors.left: fees.left
        anchors.leftMargin: 0
        text_: "Fees Amount"
    }

    MaterialTextInput
    {
        id: feesAmountFiat
        text_: "Fiat Price (per coin)"
        anchors.top: feesAmount.bottom
        anchors.topMargin: 10
        anchors.left: feesAmount.left
        anchors.leftMargin: 0
    }

/////// Comments

    Text
    {
        id: commentsTxt
        anchors.top: feesTxt.top
        anchors.topMargin: 0
        anchors.left: pair2Amount.left
        anchors.leftMargin: 0
        text: "Comments"
    }

    TextArea
    {
        id: comments
        anchors.top: commentsTxt.bottom
        anchors.topMargin: 10
        anchors.left: commentsTxt.left
        anchors.leftMargin: 0
        anchors.right: pair2Amount.right
        anchors.rightMargin: 0
        anchors.bottom: feesAmountFiat.bottom
        anchors.bottomMargin: 0
        height: 100
        background: Rectangle {
            anchors.fill: parent
            color: "white"
        }
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
            var res = brokerManager.newOperation(walletsModelDeposit.getWalletID(pair1.currentIndex), walletsModelDeposit.getWalletID(pair2.currentIndex), pair1Amount.text_, p1AmountFiat.text_, pair2Amount.text_, p2AmountFiat.text_,
                        fees.textAt(fees.currentIndex), feesAmount.text_, feesAmountFiat.text_, "", type.currentText, "", "") //TODO: comments
            console.log(res)
        }
    }

}