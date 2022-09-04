import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import es.broker.components.material 1.0


Item
{
    id: dateSelector

    property var years: ["2009"]
    property string day: "1"
    property string month: "1"
    property string year: "1900"
    property string hour: "1"
    property string minute: "1"
    property string second: "1"

    Text
    {
        id: dateTxt
        anchors.left: parent.left
        anchors.leftMargin: 0
        text: "Date DD/MM/YYYY"
    }

    ComboBox
    {
        id: day
        anchors.top: dateTxt.bottom
        anchors.topMargin: 10
        anchors.left: dateTxt.left
        anchors.leftMargin: 0
        currentIndex: 0
        width: 80
        model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                "11", "12", "13", "14", "15", "16", "17", "18", "19",
                "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"]
        onActivated: dateSelector.day = currentText
    }

    ComboBox
    {
        id: month
        currentIndex: 0
        width: 80
        anchors.top: day.top
        anchors.topMargin: 0
        anchors.left: day.right
        anchors.leftMargin: 5
        model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10","11","12"]
        onActivated: dateSelector.month = currentText
    }

    ComboBox
    {
        id: year
        width: 80
        anchors.left: month.right
        anchors.leftMargin: 5
        anchors.top: day.top
        anchors.topMargin: 0
        currentIndex: 0
        model: ListModel{id:years}
        onActivated: dateSelector.year = currentText
        Component.onCompleted:
        {
            for(var i = 2010; i < 2050; i++)
            {
                years.append({i})
            }
        }
    }

    Text
    {
        id: time
        anchors.top: day.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 0
        text: "Time HH/MM/SS"
    }

   ComboBox
    {
        id: hour
        anchors.top: time.bottom
        anchors.topMargin: 5
        anchors.left: day.left
        anchors.leftMargin: 0
        currentIndex: 0
        width: 80
        model: ["0","1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                "11", "12", "13", "14", "15", "16", "17", "18", "19",
                "20", "21", "22", "23"]
        onActivated: dateSelector.hour = currentText
    }

    ComboBox
    {
        id: minute
        currentIndex: 0
        width: 80
        anchors.top: hour.top
        anchors.topMargin: 0
        anchors.left: hour.right
        anchors.leftMargin: 5
        model: ListModel{id:minutes}
        onActivated: dateSelector.minute = currentText
        Component.onCompleted:
        {
            for(var i = 0; i < 60; i++)
            {
                minutes.append({i})
            }
        }
    }

    ComboBox
    {
        id: second
        width: 80
        anchors.top: minute.top
        anchors.topMargin: 0
        anchors.left: minute.right
        anchors.leftMargin: 5
        currentIndex: 0
        model: ListModel{id:seconds}
        onActivated: dateSelector.second = currentText
        Component.onCompleted:
        {
            for(var i = 0; i < 60; i++)
            {
                seconds.append({i})
            }
        }
    }




}