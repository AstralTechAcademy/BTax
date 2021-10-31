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
    width: parent.width
    height: 70

    signal dataChanged(var role, var value)

    ToolSeparator{
        width: parent.width
        orientation: Qt.Horizontal
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    //property int id_: 0
    //property alias price_ : precioCompra.text
    //property alias priceFiat : priceFiat.text
    //property alias date_ : date.text
    //property alias deposit_: invested.text
    //property alias status_: status.text
    //property alias comments_: comment.text


    ComboBox{
        id: coinNameFirst
        currentIndex: 0
        model: ["ADA", "AVAX", "BNB", "BTC", "ETH", "EUR", "SOL", "USD"]
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: 90
        height: 30
        enabled: if(statusCheck.state == "checked") { return false} else return true
    }

    ComboBox{
        id: coinNameSecond
        currentIndex: 0
        model: ["ADA", "AVAX", "BNB", "BTC", "ETH", "EUR", "SOL", "USD"]
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: coinNameFirst.right
        anchors.leftMargin: 10
        width: 90
        height: 30
        enabled: if( statusCheck.state == "checked") { return false} else return true

    }

    Text{
        text: "Invested"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: invested.top
        anchors.bottomMargin: 2
        anchors.left: invested.left
        anchors.leftMargin: 0
    }

    TextInput{
        id: invested
        text: deposit
        color: "red"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: coinNameSecond.right
        anchors.leftMargin: 30
        width: 150
        height: 20
        enabled: if(statusCheck.state == "checked") { return false} else return true
        onAccepted: {focus = false; deposit = text}
    }

    Text{
        text: "Price ( " + coinNameSecond.currentText + " )"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: precioCompra.top
        anchors.bottomMargin: 2
        anchors.left: precioCompra.left
        anchors.leftMargin: 0
    }


    TextInput{
        id: precioCompra
        text: price
        color: "black"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: invested.right
        anchors.leftMargin: 10
        width: 150
        height: 20
        enabled: if(statusCheck.state == "checked") { return false} else return true
        onAccepted: focus = false
    }

    Text{
        text: "Price Fiat ( EUR )"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: pFiat.top
        anchors.bottomMargin: 2
        anchors.left: pFiat.left
        anchors.leftMargin: 0
    }


    TextInput{
        id: pFiat
        text: priceFiat
        color: "black"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: precioCompra.right
        anchors.leftMargin: 10
        width: 150
        height: 20
        enabled: if(statusCheck.state == "checked") { return false} else return true
        onAccepted: focus = false
    }

    Text{
        text: "Date"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: opDate.top
        anchors.bottomMargin: 2
        anchors.left: opDate.left
        anchors.leftMargin: 0
    }

    Text{
        id: opDate
        text: date//"No Date"
        font.pixelSize: 12
        color: "black"
        width: 150
        height: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: pFiat.right
        anchors.leftMargin: 10
        enabled: if(statusCheck.state == "checked") { return false} else return true
    }


    Text{
        text: "Comment"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: opDate.top
        anchors.bottomMargin: 2
        anchors.left: comment.left
        anchors.leftMargin: 0
    }

    Text{
        id: comment
        text: ""
        font.pixelSize: 12
        color: "black"
        width: 300
        height: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: stat.left
        anchors.rightMargin: 0
        enabled: if(statusCheck.state == "checked") { return false} else return true
    }

    Text{
        id: stat
        text: status //"Not Completed"
        font.pixelSize: 12
        color: "black"
        anchors.top: statusCheck.bottom
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 0
    }

    Components.IconButton{
        id: lockOoperation
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.right: statusCheck.left
                anchors.rightMargin: 10
                source: if(statusCheck.state == "checked") return "qrc:/assets/lock-black"; else "qrc:/assets/lock-open-black";
        onClicked: {
            operation.state ="disabled"
        }
        color_: "black"
    }

    Rectangle
    {
        id: statusCheck
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 0
        width: 20
        height: 20
        radius: width * 0.5
        state: "notChecked"
        states: [
            State {
                name: "checked"
                PropertyChanges { target: statusCheck; color: "green"; border.color: "black"}
            },
            State {
                name: "notChecked"
                PropertyChanges { target: statusCheck; color: "#00000000"; border.color: "black"}
            }
        ]

        MouseArea{
            id: statusCheckMA
            anchors.fill: statusCheck
            onClicked: {
                if(statusCheck.state == "checked")
                {
                    statusCheck.state = "notChecked"
                    status_ = "Not Completed"
                }else
                {
                    statusCheck.state = "checked"
                    status_ = "Completed"
                }
            }
        }

    }

    /*ToolSeparator{
        id: buySeparator
        orientation: Qt.Vertical
        anchors.left: status.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10
    }*/
}