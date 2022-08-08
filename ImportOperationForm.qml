import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components 1.0
import es.broker.components.material 1.0

Window
{
    id:importOperationWindow

    visible: true
    height: 700
    width: 800
    //visibility: "FullScreen"
    color: "#e9e7e7"
    title: qsTr("Import")

    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2

    Component.onCompleted: console.log("Import Operation Form")

    signal close()

    Text
    {
        id: exchangeTxt
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: exchange.left
        anchors.leftMargin: 0
        text: "Exchange"
    }

    ComboBox
    {
        id: exchange
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: exchange.right
        anchors.leftMargin: 20
        model: ["B2M", "Binance"]
    }

    Text
    {
        id: userTxt
        anchors.top: exchangeTxt.top
        anchors.topMargin: 0
        anchors.left: user.left
        anchors.leftMargin: 0
        text: "User"
    }

    ComboBox
    {
        id: user
        anchors.top: exchange.top
        anchors.topMargin: 0
        anchors.left: exchange.right
        anchors.leftMargin: 20
        model: ["Gabriel"]
    }

    Button
    {
        id: importOperation
        anchors.top: user.top
        anchors.topMargin: 0
        anchors.left: user.right
        anchors.leftMargin: 20
        text: "Import"
        onClicked: {
            importFileDialog.visible = true
        }
    }

//////// Import Preview

TextArea
{
    id: importPre
    anchors.top: importOperation.bottom
    anchors.topMargin: 20
    anchors.left: parent.left
    anchors.leftMargin: 20
    anchors.right: parent.right
    anchors.rightMargin: 20
    anchors.bottom: accept.top
    anchors.bottomMargin: 20
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
        onClicked: brokerManager.importOperation("Gabriel", exchange.currentText, pair1.currentText,
               pair2.currentText, pair1Amount.text_, p1AmountFiat.text_, pair2Amount.text_, p2AmountFiat.text_,
               feesAmount.text_, feesAmountFiat.text_, "", type.currentText, "", "") //TODO: comments
    }



    FileDialog{
        id:importFileDialog
        visible: false
        selectFolder: false
        selectMultiple: false
        onAccepted: {
            var res = importer.import(exchange.currentText, importFileDialog.fileUrl)
            console.log("Import operation result = " + res)

            console.log("Import Details:")
            console.log("   Added: " + importer.opsAdded.length)
            console.log("   Already:" + importer.opsAlrdyAdded.length)
            console.log("   Error: " + importer.opsWithError.length)


        }
        onRejected: {
            //console.log("Canceled")
        }

    }

}