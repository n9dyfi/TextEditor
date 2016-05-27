import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    property alias title: dialog.titleText
    property string newOrOpen
    signal show(string op)
    onShow: {
        newOrOpen = op
        dialog.open()
    }
    QueryDialog {
        id: dialog
        message: qsTr("Discard changes?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        onAccepted: {
            newOrOpenConfirmed(newOrOpen)
        }
    }
}
