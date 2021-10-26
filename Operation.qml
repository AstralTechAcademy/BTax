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

    ToolSeparator{
        width: parent.width
        orientation: Qt.Horizontal
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    property int id_: 0
    property alias buyPrice_ : precioCompra.text
    //property alias buyPriceFiat_ : buyPriceFiat.text
    property alias buyDate_ : buyDate.text
    property alias sellPrice_ : precioVenta.text
    //property alias sellPriceFiat_ : sellPriceFiat.text
    property alias sellDate_ : sellDate.text
    property alias deposit_: deposit.text
    property alias retired_: retired.text
    property alias statusBuy_: statusBuy.text
    property alias statusSell_: statusSell.text


    ComboBox{
        id: coinNameFirst
        currentIndex: 0
        model: ["ADA", "AVAX", "BNB", "BTC", "ETH", "EUR", "SOL", "USD"]
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: 90
        height: 30
        enabled: if(statusSellCheck.state == "checked" || statusBuyCheck.state == "checked") { return false} else return true
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
        enabled: if(statusSellCheck.state == "checked" || statusBuyCheck.state == "checked") { return false} else return true

    }

    Text{
        text: "Invested"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: deposit.top
        anchors.bottomMargin: 2
        anchors.left: deposit.left
        anchors.leftMargin: 0
    }

    TextInput{
        id: deposit
        text: "Invested"
        color: "red"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: coinNameSecond.right
        anchors.leftMargin: 30
        width: 130
        height: 20
        enabled: if(statusBuyCheck.state == "checked") { return false} else return true
        onAccepted: focus = false
    }

    Text{
        text: "Buy Price"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: precioCompra.top
        anchors.bottomMargin: 2
        anchors.left: precioCompra.left
        anchors.leftMargin: 0
    }


    TextInput{
        id: precioCompra
        text: "PC"
        color: "black"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: deposit.right
        anchors.leftMargin: 10
        width: 60
        height: 20
        enabled: if(statusBuyCheck.state == "checked") { return false} else return true
        onAccepted: focus = false
    }

    Text{
        id: buyDate
        text: "No Date"
        font.pixelSize: 12
        color: "black"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: precioCompra.right
        anchors.leftMargin: 10
        enabled: if(statusBuyCheck.state == "checked") { return false} else return true
    }

    Text{
        id: statusBuy
        text: "Not Completed"
        font.pixelSize: 12
        color: "black"
        anchors.top: statusBuyCheck.bottom
        anchors.topMargin: 10
        anchors.left: buyDate.right
        anchors.leftMargin: 30
    }

    Components.IconButton{
        id: lockOoperation
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.right: statusBuyCheck.left
                anchors.rightMargin: 10
                source: if(statusBuyCheck.state == "checked") return "qrc:/assets/lock-black"; else "qrc:/assets/lock-open-black";
        onClicked: {
            operation.state ="disabled"
        }
        color_: "black"
    }

    Rectangle
    {
        id: statusBuyCheck
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: statusBuy.right
        anchors.rightMargin: 0
        width: 20
        height: 20
        radius: width * 0.5
        state: "notChecked"
        states: [
            State {
                name: "checked"
                PropertyChanges { target: statusBuyCheck; color: "green"; border.color: "black"}
            },
            State {
                name: "notChecked"
                PropertyChanges { target: statusBuyCheck; color: "#00000000"; border.color: "black"}
            }
        ]

        MouseArea{
            id: statusBuyCheckMA
            anchors.fill: statusBuyCheck
            onClicked: {
                if(statusBuyCheck.state == "checked")
                {
                    statusBuyCheck.state = "notChecked"
                    statusBuy_ = "Not Completed"
                }else
                {
                    statusBuyCheck.state = "checked"
                    statusBuy_ = "Completed"
                }
            }
        }

    }

    ToolSeparator{
        id: buySeparator
        orientation: Qt.Vertical
        anchors.left: statusBuy.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10
    }

    Text{
        text: "Sell Price"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: precioVenta.top
        anchors.bottomMargin: 2
        anchors.left: precioVenta.left
        anchors.leftMargin: 0
    }

    Text{
        text: "Retired"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: retired.top
        anchors.bottomMargin: 2
        anchors.left: retired.left
        anchors.leftMargin: 0
    }

    TextInput{
        id: retired
        text: "Retired"
        color: "green"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: buySeparator.right
        anchors.leftMargin: 10
        width: 130
        height: 20
        enabled: if(statusSellCheck.state == "checked") { return false} else return true
        onAccepted: focus = false
    }

    TextInput {
        id: precioVenta
        text: "PV"
        color: "black"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: retired.right
        anchors.leftMargin: 30
        width: 60
        height: 20
        enabled: if(statusSellCheck.state == "checked") { return false} else return true
        onAccepted: focus = false
    }

    Text{
        id: sellDate
        text: "No Date"
        font.pixelSize: 12
        color: "black"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: precioVenta.right
        anchors.leftMargin: 10
    }

    Text{
        id: statusSell
        text: "Not Completed"
        font.pixelSize: 12
        color: "black"
        anchors.top: statusSellCheck.bottom
        anchors.topMargin: 10
        anchors.left: sellDate.right
        anchors.leftMargin: 30
    }

    Components.IconButton{
        id: lockSell
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.right: statusSellCheck.left
                anchors.rightMargin: 10
                source: if(statusSellCheck.state == "checked") return "qrc:/assets/lock-black"; else "qrc:/assets/lock-open-black";
        onClicked: {
            operation.state ="disabled"
        }
        color_: "black"
    }

    Rectangle
    {
        id: statusSellCheck
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: statusSell.right
        anchors.rightMargin: 0
        width: 20
        height: 20
        radius: width * 0.5
        state: "notChecked"
        states: [
            State {
                name: "checked"
                PropertyChanges { target: statusSellCheck; color: "green"; border.color: "black"}
            },
            State {
                name: "notChecked"
                PropertyChanges { target: statusSellCheck; color: "#00000000"; border.color: "black"}
            }
        ]

        MouseArea{
            id: statusSellCheckMA
            anchors.fill: statusSellCheck
            onClicked: {
                if(statusSellCheck.state == "checked")
                {
                    statusSellCheck.state = "notChecked"
                    statusSell_ = "Not Completed"
                }else
                {
                    statusSellCheck.state = "checked"
                    statusSell_ = "Completed"
                }
            }
        }

    }

    ToolSeparator{
        orientation: Qt.Vertical
        anchors.left: sellDate.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10
    }



}