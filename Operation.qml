import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material


Rectangle{

    color: "#A5A5A5"
    radius: 10
    width: 300
    height: 100

    property int id_: 0
    property alias buyPrice_ : precioCompra.text
    property alias precioVenta_ : precioVenta.text
    property alias deposit_: deposit.text
    property alias retired_: retired.text
    property alias status_: status.text


    ComboBox{
        id: coinNameFirst
        currentIndex: 0
        model: ["ADA", "AVAX", "BNB", "BTC", "ETH", "EUR", "SOL", "USD"]
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: 90
        height: 30
    }

    ComboBox{
        id: coinNameSecond
        currentIndex: 0
        model: ["ADA", "AVAX", "BNB", "BTC", "ETH", "EUR", "SOL", "USD"]
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: coinNameFirst.right
        anchors.leftMargin: 10
        width: 90
        height: 30

    }

    TextInput{
        id: precioCompra
        text: "PC"
        color: "white"
        font.pixelSize: 15
        anchors.top: coinNameFirst.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
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

    TextInput {
        id: precioVenta
        text: "PV"
        color: "white"
        font.pixelSize: 15
        anchors.top: precioCompra.top
        anchors.topMargin: 0
        anchors.left: precioCompra.right
        anchors.leftMargin: 10
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
                precioVenta_ += "10"
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
                PropertyChanges { target: precioVenta; text: precioVenta_}
            },
            State {
                name: "selected"
                PropertyChanges { target: precioVenta; text: ""; focus: true}
                PropertyChanges { target: pvMA; state: "disabled"}
            }
        ]
    }

    TextInput{
        id: deposit
        text: "Deposit"
        color: "red"
        font.pixelSize: 15
        anchors.top: precioCompra.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: 130
        height: 20
    }

    TextInput{
        id: retired
        text: "Retired"
        color: "green"
        font.pixelSize: 15
        anchors.top: deposit.top
        anchors.topMargin: 0
        anchors.left: deposit.right
        anchors.leftMargin: 10
        width: 130
        height: 20
    }

    Text{
        id: status
        text: "Not Completed"
        font.pixelSize: 12
        color: "white"
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
                PropertyChanges { target: circleCheckBox; color: "green"; border.color: "white"}
            },
            State {
                name: "notChecked"
                PropertyChanges { target: circleCheckBox; color: "#00000000"; border.color: "white"}
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