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
    height: 70

    signal dataChanged(var role, var value)

    /*ToolSeparator{
        width: parent.width
        orientation: Qt.Horizontal
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }*/

    //property int id_: 0
    //property alias price_ : precioCompra.text
    //property alias priceFiat : priceFiat.text
    //property alias date_ : date.text
    //property alias deposit_: invested.text
    //property alias status_: status.text
    //property alias comments_: comment.text


    Text{
        id: opType
        text: type
        width: 50
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        color: if(type == "Venta" || type == "Withdraw") { return "red"} else {return "green"}
    }



    /*ComboBox{
        id: coinNameFirst
        currentIndex: 0
        model: ["ADA", "AVAX", "BNB", "BTC", "ETH", "EUR", "SOL", "USD"]
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: opType.right
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

    }*/

    Text{
        text: pair1
        font.pixelSize: 12
        color: "black"
        anchors.bottom: p1Amount.top
        anchors.bottomMargin: 2
        anchors.left: p1Amount.left
        anchors.leftMargin: 0
    }

    TextInput{
        id: p1Amount
        text: pair1Amount +"~" + (  pair1Amount*pair1AmountFiat).toFixed(2);
        color: "red"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: opType.right
        anchors.leftMargin: 40
        width: 150
        height: 20
        enabled: if(statusCheck.state == "checked") { return false} else return true
        onAccepted: {focus = false; }
    }

    Text{
        text: pair1 + "/EUR"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: p1AmountFiat.top
        anchors.bottomMargin: 2
        anchors.left: p1AmountFiat.left
        anchors.leftMargin: 0
    }

    TextInput{
        id: p1AmountFiat
        text: pair1AmountFiat
        color: "red"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: p1Amount.right
        anchors.leftMargin: 30
        width: 150
        height: 20
        enabled: if(statusCheck.state == "checked") { return false} else return true
        onAccepted: {focus = false; }
    }

    Text{
        text: pair2
        font.pixelSize: 12
        color: "black"
        anchors.bottom: p2Amount.top
        anchors.bottomMargin: 2
        anchors.left: p2Amount.left
        anchors.leftMargin: 0
    }


    TextInput{
        id: p2Amount
        text: pair2Amount +"~" + (pair2Amount*pair2AmountFiat).toFixed(2);
        color: "black"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: p1AmountFiat.right
        anchors.leftMargin: 10
        width: 150
        height: 20
        enabled: if(statusCheck.state == "checked") { return false} else return true
        onAccepted: focus = false
    }

    Text{
        text: pair2 + "/EUR"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: pFiat.top
        anchors.bottomMargin: 2
        anchors.left: pFiat.left
        anchors.leftMargin: 0
    }


    TextInput{
        id: pFiat
        text: pair2AmountFiat
        color: "black"
        font.pixelSize: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: p2Amount.right
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
        text: "Ganancia"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: opDate.top
        anchors.bottomMargin: 2
        anchors.left: ganan.left
        anchors.leftMargin: 0
    }

    Text{
        id: ganan
        text: ganancia
        font.pixelSize: 12
        color: if(ganancia >= 0)  { return "green"} else return "red"
        width: 150
        height: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: opDate.right
        anchors.leftMargin: 10
        enabled: if(statusCheck.state == "checked") { return false} else return true
    }

    Text{
        text: "Comment"
        font.pixelSize: 12
        color: "black"
        anchors.bottom: ganan.top
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
                    status = "Not Completed"
                }else
                {
                    statusCheck.state = "checked"
                    status   = "Completed"
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