import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14


Rectangle{
    id: rect
    width: 280
    height: 50

    signal selected()
    signal accepted()
    property string id_
    property alias text_: textInput.text
    property bool disabled_: false
    property var colorLine_: "#fbbb21"

    color: "white"


    Component.onCompleted:{
        oldValue.state = "enabled"
        textInput.state = "enabled"
    }

    Rectangle
    {
        id: line
        color: colorLine_
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

        states: [
            State {
                name: "disabled"
                PropertyChanges { target: oldValue; text: id_; color: "white" }
            },
            State {
                name: "enabled"
                PropertyChanges { target: oldValue; text: ""; color: "#03657c" }
            },
            State {
                name: "selected"
                PropertyChanges { target: oldValue; text: id_; color: "#03657c"}
            },
            State {
                name: "default"
                PropertyChanges { target: oldValue; text: ""; color: "#03657c"}
            }
        ]
    }

    TextInput{
        id: textInput
        
        anchors.fill: parent
        anchors.top: oldValue.bottom
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 10
        enabled: true
        color: "lightgray"
        text:  text_
        verticalAlignment: TextInput.AlignVCenter
        onAccepted: {
            oldValue.state = "enabled"
            textInput.state = "enabled"
            rect.accepted()
        }

        MouseArea
        {
            id: ma
            anchors.fill: textInput
            enabled: true
            onClicked: {
                oldValue.state = "selected"
                textInput.state = "selected"
                //textInput.color = "black"
                //text_ = ""
                rect.selected()
            }

            states: [
                State {
                    name: "disabled"
                    PropertyChanges { target: ma; enabled: false; visible: false}
                },
                State {
                    name: "enabled"
                    PropertyChanges { target: ma; enabled: true; visible: true }
                },
                State {
                    name: "default"
                    PropertyChanges { target: ma; enabled: true; visible: true }
                }
            ]
        }
        states: [
            State {
                name: "disabled"
                PropertyChanges { target: textInput; text: text_; color: "white"}
                PropertyChanges { target: ma; state: "disabled"}
            },
            State {
                name: "enabled"
                PropertyChanges { target: textInput; text: text_; color: "lightgray"}
                PropertyChanges { target: ma; state: "enabled"}
            },
            State {
                name: "selected"
                PropertyChanges { target: textInput; text: ""; color: "black"; focus: true}
                PropertyChanges { target: ma; state: "disabled"}
            },
            State {
                name: "default"
                PropertyChanges { target: textInput; text: id_; color: "lightgray"; enabled: true; focus: false}
                PropertyChanges { target: ma; state: "default"}
            }
        ]
    }

    states: [
        State {
            name: "disabled"
            PropertyChanges { target: oldValue; state: "disabled" }
            PropertyChanges { target: textInput; state: "disabled"}
            PropertyChanges { target: rect; color: "lightgray"}
        },
        State {
            name: "enabled"
            PropertyChanges { target: textInput; state: "enabled" }
            PropertyChanges { target: oldValue; state: "enabled" }
            PropertyChanges { target: rect; color: "white"}
        }
    ]

    function clear()
    {
         text_ = id_
         oldValue.state = "default"
         textInput.state = "default"
         ma.state = "default"
         rect.state = "enabled"
    }


}