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

    Row
    {
        id: wallets

        Repeater
        {
            model: walletsModel
            delegate: Wallet
        }


    }




}