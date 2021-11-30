import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components.material 1.0 as Material
import es.broker.components 1.0 as Components


Rectangle{
    id: operation
    width: parent.width
    color: "#00000000"//"#A5A5A5"
    radius: 10

    ToolSeparator{
        width: parent.width
        orientation: Qt.Horizontal
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }


    Text{
        id: id
        text: coin
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }

    Text{
        id: exchangeTxt
        text: exchange
        anchors.left: id.right
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        width: 80
    }

    Components.WalletData
    {
        id: amountTxt
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: exchangeTxt.right
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        width: 80
        visible: true
        title: "Amount"
        value: parseFloat( amount.toFixed(6) )
        colorValue: "black"
    }

    Components.WalletData
    {
        id: investedTxt
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: amountTxt.right
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        width: 80
        visible: true
        title: "Invested"
        value: parseFloat( invested.toFixed(6) )
        colorValue: "black"
    }


    Components.WalletData
    {
        id: averageTxt
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: investedTxt.right
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        width: 80
        visible: true
        title: "Average"
        value: parseFloat( average.toFixed(6) )
        colorValue: "black"
    }


/////// EARNING

    Components.WalletData
    {
        id: earning
        anchors.left: averageTxt.right
        anchors.leftMargin: 30
        anchors.verticalCenter: parent.verticalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        width: 80
        visible: false
        title: "Earning"
        value: {
          if((amount * currentPriceTxt.text_ - invested) > 0 )
          {
               return "+" +  parseFloat( ( (amount * currentPriceTxt.text_) - invested ).toFixed(3) )
          }
          else
          {
               return "-" +  parseFloat( ( (amount * currentPriceTxt.text_) - invested ).toFixed(3) )
          }
        }
        colorValue: if((amount * currentPriceTxt.text_ - invested) > 0 ) {return "green"} else {return "red"}
    }

    Components.WalletData
    {
        id: percentage
        anchors.left: earning.right
        anchors.leftMargin: 30
        anchors.verticalCenter: parent.verticalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        width: 80
        visible: false
        title: "Percentage"
        value: {
          if((amount * currentPriceTxt.text_ - invested) > 0 )
          {
               return "+" +  parseFloat( ( (amount * currentPriceTxt.text_ * 100) / invested).toFixed(3) )+ "%"
          }
          else
          {
               return "-" + parseFloat(( (amount * currentPriceTxt.text_)  / invested).toFixed(3) )+ "%"
          }
        }
        colorValue: if((amount * currentPriceTxt.text_ - invested) > 0 ) {return "green"} else {return "red"}
    }



    Material.TextInput{
        id: currentPriceTxt
        text_: "Price (€)"
        anchors.right: parent.right
        anchors.rightMargin: 100
        anchors.verticalCenter: parent.verticalCenter
        colorLine_: "white"
        width: 100
        height: 40
        onAccepted: {
            earning.visible = true
            percentage.visible = true
        }
    }


}