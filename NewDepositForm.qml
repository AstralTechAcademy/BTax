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

    Component.onCompleted: console.log("New Deposit Form")

    Text
    {
        id: exchangeTxt
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
        text: "Exchange"
    }

    ComboBox
    {
        id: exchange
        anchors.top: exchangeTxt.bottom
        anchors.topMargin: 10
        anchors.left: exchangeTxt.left
        anchors.leftMargin: 0
        model: ["B2M", "Binance"]
    }

//////////PAIR 1

    Text
    {
        id: pair1Txt
        anchors.top: exchange.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 50
        text: "Pair 1 (Without fees)"
    }

    ComboBox
    {
        id: pair1
        anchors.top: pair1Txt.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 50
        currentIndex: 0
        model: ["EUR","USD"]
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
        onClicked: brokerManager.newDeposit("Gabriel", exchange.currentText, pair1.currentText,
               pair1Amount.text_, feesAmount.text_, "", "") //TODO: comments
    }

}