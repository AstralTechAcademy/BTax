import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import Broker 1.0


Rectangle{
    id: rect
    width: 350
    height: 50

    property alias text_: textInput.text
    Rectangle
    {
        id: line
        color: "#fbbb21"
        width: 5 
        height: rect.height
        anchors.left: rect.left
        anchors.leftMargin: 0
    }

    Text {
        id: oldValue
        text : ""
        color: "#03657c"
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10
    
    }

    TextInput{
        id: textInput
        
        anchors.fill: parent
        anchors.top: oldValue.bottom
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 10
        color: "lightgray"
        text:  text_
        verticalAlignment: TextInput.AlignVCenter

        MouseArea
        {
            id: ma
            anchors.fill: textInput
            onClicked: {
                oldValue.text = text_
                textInput.color = "black"
                text_ = ""
                ma.visible = false
                textInput.focus = true
            }
        }                
    }  
}