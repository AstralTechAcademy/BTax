import QtQuick 2.2
import QtQuick.Dialogs 1.1


MessageDialog {
    id: messageDialog
    onAccepted: {
        console.log("And of course you could only agree.")
        Qt.quit()
    }
}
