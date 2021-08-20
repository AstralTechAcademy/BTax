import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14

Item{

    id: body

        Item{
            id: inputsItem
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            width: 350
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            Column
            {
                id: inputs
                spacing: 15

                MaterialTextInput{
                    id: pc
                    text_: "Precio Compra (PC)"

                }
                        
                MaterialTextInput{
                    id: invertido
                    text_: "Invertido (€)"
                }
                    
                MaterialTextInput
                {
                    id: pv
                    text_: "Precio Venta (PV)"
                }

                MaterialTextInput{
                    id: recuperado
                    text_: "Recuperado (€)"
                }                
            }

            Rectangle{
                id: addButton
                color: "blue"
                anchors.top: inputs.top
                anchors.topMargin: 40
                anchors.left: inputs.right
                anchors.leftMargin: 15
                width: 30
                height: 30
                radius: width*0.5
                Text{
                    text: "+"
                    color: "white"
                    font.pixelSize: 15
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                MouseArea
                {
                    id: addMA
                    anchors.fill: parent
                    onClicked: console.log("Add")
                }
            }
        }


       
        Button{
            id: calcularButton
            text: "Calcular"
            onClicked: {recuperado.text_ = pv.text_/pc.text_ * invertido.text_}
        }
}