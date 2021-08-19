import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14

Item{

    id: body

        MaterialTextInput
        {
            id: pv
            text_: "Precio Venta (PV)"
        }

        MaterialTextInput{
            id: pc
            text_: "Precio Compra (PC)"
            anchors.top: pv.bottom
            anchors.topMargin: 20
        }
        
        MaterialTextInput{
            id: invertido
            text_: "Invertido (€)"
            anchors.top: pc.bottom
            anchors.topMargin: 20
        }

        MaterialTextInput{
            id: recuperado
            text_: "Recuperado (€)"
            anchors.top: invertido.bottom
            anchors.topMargin: 20
        }

        Button{
            id: calcularButton
            text: "Calcular"
            onClicked: {recuperado.text_ = pv.text_/pc.text_ * invertido.text_}
        }
}