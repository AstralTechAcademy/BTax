import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14


Rectangle{
    id: rect
    width: 350
    height: 50

    signal selected()
    property string id_
    property alias text_: textInput.text
    property bool disabled_: false

    color: if(disabled_ == false){"white"}else{"lightgray"}

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
        text : if(disabled_ == false){""}else{id_}
        color: if(disabled_ == false){"#03657c"}else{"white"}
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
        enabled: if(disabled_ == false){true}else{false}
        color: if(disabled_ == false){"lightgray"}else{"white"}
        text:  text_
        verticalAlignment: TextInput.AlignVCenter

        MouseArea
        {
            id: ma
            anchors.fill: textInput
            enabled: if(disabled_ == false){true}else{false}
            onClicked: {
                oldValue.text = text_
                textInput.color = "black"
                text_ = ""
                ma.visible = false
                textInput.focus = true
                rect.selected()
            }
        }                
    }

    function clear()
    {
         text_ = id_
         disabled_ = false
         oldValue.text = ""
//         if(disabled_ == false){ oldValue.color = "#03657c"}else{  oldValue.color = "white"}
  //       if(disabled_ == false){textInput.color =  "lightgray"}else{ textInput.color =  "white"}
         textInput.focus = false
         ma.enabled = true
         ma.visible = true
    }
}