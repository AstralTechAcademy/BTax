import QtQuick 2.12
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.12

Image
{
    width: 20
    height: 20
    signal clicked()
    property string color_: ""

    ColorOverlay {
        anchors.fill: parent
        source: parent
        color: color_
    }

     MouseArea{
        anchors.fill: parent
        onClicked: parent.clicked()
     }

}


