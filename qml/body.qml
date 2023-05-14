import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material
import es.broker 1.0

Item{

    id: body

        Item{
            id: inputsItem
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            width: 300
            anchors.bottom: addButton.top
            anchors.bottomMargin: 20
            Column
            {
                id: inputs
                spacing: 15

                Material.TextInput{
                    id: pc
                    id_: "Precio Compra (PC)"
                    text_: "Precio Compra (PC)"

                }
                        
                Material.TextInput{
                    id: invertido
                    id_: "Invertido (€)"
                    text_: "Invertido (€)"
                }

                Material.TextInput{
                    id: benefitWaited
                    id_: "Beneficio (%)"
                    text_: "Beneficio (%)"
                    onSelected: {
                        pv.state = "disabled"
                        pv.text_ = ""
                    }
                }
                    
                Material.TextInput
                {
                    id: pv
                    id_: "Precio Venta (PV)"
                    text_: "Precio Venta (PV)"
                    onSelected: {
                        benefitWaited.state = "disabled"
                        benefitWaited.text_ = ""
                    }
                }

                Material.TextInput{
                    id: recuperado
                    id_: "Recuperado (€)"
                    text_: "Recuperado (€)"
                    onSelected: {
                        pv.state = "disabled"
                        pv.text_ = ""
                        benefitWaited.state = "disabled"
                        benefitWaited.text_ = ""
                    }
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
            anchors.left: inputsItem.left
            anchors.leftMargin: 5
            anchors.bottom: body.bottom
            anchors.bottomMargin: 10
            onClicked: {
                calculate(pc,invertido, benefitWaited, pv, recuperado)
            }
        }

        Button{
            id: clearButton
            text: "Limpiar"
            anchors.right: inputsItem.right
            anchors.rightMargin: 5
            anchors.bottom: body.bottom
            anchors.bottomMargin: 10
            onClicked: {
                pc.clear()
                invertido.clear()
                benefitWaited.clear()
                pv.clear()
                recuperado.clear()
            }
        }

        function calculate(pc, invertido, benefitWaited, pv, recuperado){

            if(pv.disabled_ && !recuperado.disabled_ && !benefitWaited.disabled_)
            {
                recuperado.text_ = Number(invertido.text_) + Number(invertido.text_*benefitWaited.text_/100)
                pv.text_ = recuperado.text_/invertido.text_*pc.text_
            }
            else if(benefitWaited.disabled_ && !recuperado.disabled_ && !pv.disabled_ )
            {
                recuperado.text_ = pv.text_/pc.text_ * invertido.text_;
                benefitWaited.text_ = Number (Number(recuperado.text_) - Number(invertido.text_))*100 / Number(invertido.text_)
            }
            else if(benefitWaited.disabled_ && pv.disabled_ )
            {
                benefitWaited.text_ = Number (Number(recuperado.text_) - Number(invertido.text_))*100 / Number(invertido.text_)
                pv.text_ = Number(recuperado.text_/invertido.text_)*Number(pc.text_)
            }
            else
            {
                recuperado.text_ = pv.text_/pc.text_ * invertido.text_;
            }
        }

}