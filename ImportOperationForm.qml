import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14
import Astral 1.0
import es.broker 1.0
import es.broker.components 1.0
import es.broker.components.material 1.0

Window
{
    id:importOperationWindow

    visible: true
    height: 400
    width: 600
    //visibility: "FullScreen"
    color: "#e9e7e7"
    title: qsTr("Import")

 
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2

    Connections
    {
        target: importer
        onImported: mutableBtn.text = "Read"
        onNotImported: mutableBtn.text = "Read"           
        onReading: {
            newOp.value = "0"
            alreadyOp.value = "0"
            unknownOp.value = "0"

            mutableBtn.text = "Import"
            loading.visible = true
            dropA.opacity = 0.5
            mutableBtn.enabled = false
        } 
        onRead: {
            loading.visible = false
            newOp.value = importer.opsAddedSize()
            alreadyOp.value = importer.opsAlrdyAddedSize()
            unknownOp.value = importer.opsWithErrorSize()
            mutableBtn.enabled = true
        }

    }

    Component.onCompleted: {
        console.log("Import Operation Form")
        importer.previewed = false
    }

    signal close()

    Data
    {
        id: newOp
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        width: 150   
        height: 50
        title: "New Operations"
        value: "0"
        backgroundColor: "lightgrey"
    }

    Data
    {
        id: alreadyOp
        anchors.top: newOp.top
        anchors.topMargin: 0
        anchors.left: newOp.right
        anchors.leftMargin: 20    
        width: 150  
        height: 50                
        title: "Already Operations"
        value: "0"
        backgroundColor: "lightgrey"        
    }

    Data
    {
        id: unknownOp
        anchors.top: alreadyOp.top
        anchors.topMargin: 0
        anchors.left: alreadyOp.right
        anchors.leftMargin: 20   
        width: 150   
        height: 50                       
        title: "Unknown Operations"
        value: "0"
        backgroundColor: "lightgrey"        
    }        

    DropArea {
        id: dropA
        anchors.top: parent.top
        anchors.topMargin: parent.height / 2
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: mutableBtn.top
        anchors.bottomMargin: 20

        property bool dropped: false

        onDropped: function(drop)
        {
            console.log(drop.urls)
            importer.uris = drop.urls
            console.log(importer.rowCount())

            dropped = true

        }

        Rectangle {
            anchors.fill: parent
            border.color: "green"
            border.width: 3
            color: "transparent"
            radius: 5
            visible: true

            Image
            {
                visible: !dropA.dropped
                source: "qrc:assets/upload-file-black"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter             
            }

            ListView
            {
                id: files
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 60
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10   
                             
                model: importer
                spacing: 2
                orientation: Qt.Horizontal
                visible: dropA.dropped
                delegate: Item {
                    width: img.width + name.width / 1.5
                    height: img.height + 10
                    Image
                    {
                        id: img
                        source: "qrc:assets/upload-file-black"
                    }

                    Text{
                        id: name
                        anchors.horizontalCenter: img.horizontalCenter
                        anchors.top: img.bottom
                        anchors.topMargin: 1
                        text: model.Path
                    }
                }

            }

        }
    }

    ALoading
    {
        id: loading
        
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
    }

/////// Bottom

    Button
    {
        id: mutableBtn
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Read"
        onClicked: {
            if(importer.previewed == false)
            {
                console.log("Reading")
                importer.reading()
                importer.importPreview()
            }
            else
            {
                console.log("Import")
                importer.import()
            }

        }
    }

    FileDialog{
        id:importFileDialog
        visible: false
        onAccepted: importer.import(exchange.currentText, importFileDialog.fileUrl)
        onRejected: {
            //console.log("Canceled")
        }

    }

}