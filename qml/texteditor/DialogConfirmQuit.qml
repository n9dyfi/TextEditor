import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    property alias title: dialog.titleText
    signal show
    onShow: dialog.open()
    QueryDialog {
        id: dialog
        message: qsTr("Save before exiting?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        onAccepted: {
            appIsClosing = true
            saveBeforeClosed(editPage.content)
        }
        onRejected: {
            appToBeClosed()
        }
    }
}
