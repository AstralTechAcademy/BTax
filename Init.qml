import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs
import QtQuick.Controls 2.14
import es.notifications 1.0
import es.broker 1.0
import Astral 1.0

Window {
    id: initWindow
    visible: true
    height: 400
    width: 600
    //flags: Qt.FramelessWindowHint
    //visibility: "FullScreen"
    color: "#e9e7e7"
    title: qsTr("Broker Loading")

    onClosing: BrokerImpl.close()

    Component.onCompleted: {
            setX(Screen.width / 2 - width / 2);
            setY(Screen.height / 2 - height / 2);
            footer.text = "Loading..."

            BrokerImpl.openDatabase()
   }


    Connections{
        target: notificationManager

        function onDbNotOpened(){
                footer.text = "Database not opened " +  BrokerImpl.getServer()
        }
    }

    Connections{
        target: BrokerImpl

        function onConnectingDatabase()
        {
            footer.text = "Connecting Database..."
        }

        function onNotOpened()
        {
            footer.text = "Database not opened " +  BrokerImpl.getServer()
        }

        function onConfigNotValid()
        {
            footer.text = "Config file not found or invalid"

        }

        function onOpened()
        {
            footer.text = "Loading data..."
            BrokerImpl.load()
        }

        function onLoaded(){
                footer.text = "Loaded. Connection with database " +  BrokerImpl.getServer()
                winld.active = true
                winld.source = "main.qml"
                initWindow.visible = false
        }

        function onNoUsers(){
            footer.text = "No users in database. Please, create one before continue."
            userTxt.visible = true
            create.visible = true
        }

        function onUserAlready(){
            footer.text = "The user is already in database"
        }
    }

    Text
    {
        id: footer
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
    }

    Loader {
        id: winld
        active: false
    }

     Connections{
        target: winld.item
        function onClosing () { BrokerImpl.close(); }
     }

    ATextInput
    {
        id: userTxt
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        visible:false
        text_: "Username"
        lineColor_: "black"
    }

    Button
    {
        id: create
        anchors.top: userTxt.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Create"
        visible: false
        onClicked: {
            var r = BrokerImpl.newUser(userTxt.text_)
            console.log("Create User " + userTxt.text_ + " " + r)
        }
    }
}