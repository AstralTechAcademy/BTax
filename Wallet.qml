import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material
import es.broker.components 1.0 as Components


Rectangle{
    id: operation
    color: "#00000000"//"#A5A5A5"
    radius: 10

    ToolSeparator{
        width: parent.width
        orientation: Qt.Horizontal
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }


    Text{
        id: id
        text: coin
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
    }

    Text{
        id: exchangeTxt
        text: exchange
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: id.right
        anchors.leftMargin: 10
    }

    Text{
        id: amountTxt
        text: "Amount: " + amount
        anchors.top: id.bottom
        anchors.topMargin: 10
        anchors.left: id.left
        anchors.leftMargin: 10
    }

    Text{
        id: investedTxt
        text: "Invested: " + invested
        anchors.top: amountTxt.top
        anchors.topMargin: 0
        anchors.left: amountTxt.right
        anchors.leftMargin: 10
    }

    Text{
        id: averageTxt
        text: "Average: " + average
        anchors.top: amountTxt.top
        anchors.topMargin: 0
        anchors.left: investedTxt.right
        anchors.leftMargin: 10
    }


}