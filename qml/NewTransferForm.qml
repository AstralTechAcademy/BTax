import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14
import es.broker.components 1.0
import es.broker.components.material 1.0

Window
{
    id:newTransferWindow

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

/////// Date

    DateSelector
    {
        id: dateSelector
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 50
        width: 300
    }

////////// WALLET ORIGIN
    Text
    {
        id: walletOTxt
        anchors.top: parent.top
        anchors.topMargin: 200
        anchors.left: parent.left
        anchors.leftMargin: 50
        text: "Wallet Origin (Without fees)"
    }

    ComboBox
    {
        id: walletO
        anchors.top: walletOTxt.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 50
        width: 200
        currentIndex: 0
        model: walletsModelAll
        textRole: "display"
        popup.onClosed: console.log("Combo Cliked " + walletsModelAll.getWalletID(currentIndex))
    }

    MaterialTextInput
    {
        id: walletOAmount
        anchors.top: walletO.bottom
        anchors.topMargin: 10
        anchors.left: walletO.left
        anchors.leftMargin: 0
        text_: "Amount"

    }

////////// WALLET DESTINATION

    Text
    {
        id: walletDTxt
        anchors.top: walletOTxt.top
        anchors.topMargin: 0
        anchors.left: walletOAmount.right
        anchors.leftMargin: 30
        text: "Wallet Destination (Without fees)"
    }

    ComboBox
    {
        id: walletD
        anchors.top: walletDTxt.bottom
        anchors.topMargin: 10
        anchors.left: walletDTxt.left
        anchors.leftMargin: 0
        model: walletsModelAll
        width: 200
        textRole: "display"
        popup.onClosed: console.log("Combo Cliked " + walletsModelAll.getWalletID(currentIndex))
    }

    MaterialTextInput
    {
        id: walletDAmount
        anchors.top: walletD.bottom
        anchors.topMargin: 10
        anchors.left: walletD.left
        anchors.leftMargin: 0
        text_: "Amount"
    }

//////////Fees

    Text
    {
        id: feesTxt
        anchors.top: walletOAmount.bottom
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
        anchors.left: walletDAmount.left
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
        anchors.right: walletDAmount.right
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
            console.log(dateSelector.day + "/" + dateSelector.month + "/" + dateSelector.year + "/  " +
                            dateSelector.hour + ":" + dateSelector.minute + ":" + dateSelector.second)
            var dateTime = ""
            if (dateSelector.year != 1900)
                dateTime = dateSelector.day + "/" + dateSelector.month + "/" + dateSelector.year + " " +
                           dateSelector.hour + ":" + dateSelector.minute + ":" + dateSelector.second
            if(walletOAmount.text_ == "" || walletOAmount.text_ == "Amount" )
                walletOAmount.text_ = "-0.1"
            if(walletDAmount.text_ == "" || walletDAmount.text_ == "Amount" )
                walletOAmount.text_ = "-0.1"
            if(feesAmount.text_ == "" || feesAmount.text_ == "Fees Amount" )
                feesAmount.text_ = "-0.1"
            if(feesAmountFiat.text_ == "" || feesAmountFiat.text_ == "Fiat Price (per coin)" )
                feesAmountFiat.text_ = "-0.1"
            var res = brokerManager.newTransfer(walletsModelAll.getWalletID(walletO.currentIndex), walletsModelAll.getWalletID(walletD.currentIndex), walletOAmount.text_, walletDAmount.text_, 
                                                    fees.textAt(fees.currentIndex), feesAmount.text_, feesAmountFiat.text_, "", "", dateTime) // TODO: Comments
            console.log(res)
            newTransferWindow.close()
        }
    }

}