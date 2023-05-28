import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14
import es.broker.components.material 1.0
import Astral 1.0

Window
{
    id:newAsset

    visible: true
    height: 640
    width: 800
    //visibility: "FullScreen"
    color: "#e9e7e7"
    title: qsTr("New Asset")

    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2

    Component.onCompleted: {
        console.log("New Asset Form")
    }

    signal close()

    ComboBox
    {
        id: assetType
        model: assetTypesModel
        textRole: "name"
       anchors.top: parent.top
       anchors.topMargin: 120
       anchors.left: assetTxt.right
       anchors.leftMargin: 20
       popup.onClosed: console.log("Combo Cliked " + currentIndex + " " + assetTypesModel.getName(currentIndex))

    }

    ATextInput
    {
        id: assetTxt
        anchors.top: parent.top
        anchors.topMargin: 120
        anchors.left: parent.left
        anchors.leftMargin: 50
        text_: "Asset Name"
        lineColor_: "black"
    }

    ATextInput
    {
        id: colorTxt
        anchors.top: assetTxt.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 50
        text_: "Color Hex code"
        lineColor_: "black"
    }

/////// Bottom

    Button
    {
        id: accept
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Accept"
        onClicked: {
            var r = brokerManager.newAsset(assetTypesModel.getName(assetType.currentIndex),assetTxt.text_, colorTxt.text_) //TODO: comments
            console.log("Asset añadido")
            newAsset.close()
        }
    }

}