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


    Text{
        id: opType
        text: type
        width: 50
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        color: if(type == "Venta" || type == "Withdraw") { return "red"} else {return "green"}
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

}