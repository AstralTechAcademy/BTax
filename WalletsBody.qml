import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material
import es.broker 1.0

Item
{

    id: walletsBody
    anchors.fill: parent

    Text
    {
        id: totalInvested
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: walletsBody.left
        anchors.leftMargin: 20
        anchors.right: walletsBody.right
        anchors.rightMargin: 10
        text: "Total Invested: " + walletsModel.getTotalInvested() + " " + "EUR"
        font.pixelSize: 20
    }



    Column
    {
        id: wallets
        anchors.top: totalInvested.bottom
        anchors.topMargin: 20
        anchors.left: walletsBody.left
        anchors.leftMargin: 10
        anchors.right: walletsBody.right
        anchors.rightMargin: 10

        Repeater
        {
            Component.onCompleted: console.log(walletsModel.rowCount())
            model: walletsModel
            delegate: Wallet{
            anchors.left: wallets.left
            anchors.leftMargin: 10
            anchors.right: wallets.right
            anchors.rightMargin: 10
            height: 70
            }
        }


    }




}