import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    property alias title: dialog.titleText
    property alias message: dialog.message
    signal show
    onShow: dialog.open()
    QueryDialog {
        id: dialog
        acceptButtonText: qsTr("OK")
    }
}
