import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material


Rectangle{

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
    property alias status_: status.text


    ComboBox{
        id: coinNameFirst
        currentIndex: 0
        model: ["ADA", "AVAX", "BNB", "BTC", "ETH", "EUR", "SOL", "USD"]
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: 90
        height: 30
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
        enabled: true
        onAccepted: pcMA.state = "enabled"

        MouseArea
        {
            id: pcMA
            anchors.fill: precioCompra
            enabled: true
            onClicked: {
                console.log("Clicked")
                buyPrice_ += "10"
                pcMA.state = "disabled"
            }

            state: "enabled"
            states: [
                State {
                    name: "enabled"
                    PropertyChanges { target: pcMA; enabled: true}
                    PropertyChanges { target: precioCompra; state: "default"}
                },
                State {
                    name: "disabled"
                    PropertyChanges { target: pcMA; enabled: false}
                    PropertyChanges { target: precioCompra; state: "selected"}
                }
            ]
        }

        state: "default"
        states: [
            State {
                name: "default"
                PropertyChanges { target: precioCompra; text: buyPrice_}
            },
            State {
                name: "selected"
                PropertyChanges { target: precioCompra; text: ""; focus: true}
                PropertyChanges { target: pcMA; state: "disabled"}
            }
        ]

    }

    Text{
        id: buyDate
        text: "No Date"
        font.pixelSize: 12
        color: "black"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: precioCompra.right
        anchors.leftMargin: 10
    }

    ToolSeparator{
        orientation: Qt.Vertical
        anchors.left: buyDate.right
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
        anchors.left: buyDate.right
        anchors.leftMargin: 30
        width: 130
        height: 20
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
        enabled: true
        onAccepted: pvMA.state = "enabled"

        MouseArea
        {
            id: pvMA
            anchors.fill: precioVenta
            enabled: true
            onClicked: {
                console.log("Clicked")
                sellPrice_ += "10"
                pvMA.state = "disabled"
            }

            state: "enabled"
            states: [
                State {
                    name: "enabled"
                    PropertyChanges { target: pvMA; enabled: true}
                    PropertyChanges { target: precioVenta; state: "default"}
                },
                State {
                    name: "disabled"
                    PropertyChanges { target: pvMA; enabled: false}
                    PropertyChanges { target: precioVenta; state: "selected"}
                }
            ]
        }

        state: "default"
        states: [
            State {
                name: "default"
                PropertyChanges { target: precioVenta; text: sellPrice_}
            },
            State {
                name: "selected"
                PropertyChanges { target: precioVenta; text: ""; focus: true}
                PropertyChanges { target: pvMA; state: "disabled"}
            }
        ]
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

    ToolSeparator{
        orientation: Qt.Vertical
        anchors.left: sellDate.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10
    }



    Text{
        id: status
        text: "Not Completed"
        font.pixelSize: 12
        color: "black"
        anchors.top: circleCheckBox.bottom
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
    }

    Rectangle
    {
        id: circleCheckBox
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        width: 20
        height: 20
        radius: width * 0.5
        state: "notChecked"
        states: [
            State {
                name: "checked"
                PropertyChanges { target: circleCheckBox; color: "green"; border.color: "black"}
            },
            State {
                name: "notChecked"
                PropertyChanges { target: circleCheckBox; color: "#00000000"; border.color: "black"}
            }
        ]

        MouseArea{
            id: circleCheckBoxMA
            anchors.fill: circleCheckBox
            onClicked: {
                if(circleCheckBox.state == "checked")
                {
                    circleCheckBox.state = "notChecked"
                    status_ = "Not Completed"
                }else
                {
                    circleCheckBox.state = "checked"
                    status_ = "Completed"
                }
            }
        }

    }



}