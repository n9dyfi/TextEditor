import QtQuick 1.1
import com.nokia.meego 1.0
import "appDefaults.js" as AppDefaults

Item {
    signal show
    onShow: dialog.open()
    QueryDialog {
        id: dialog
        titleText: "TextEditor "+AppDefaults.VERSION
        message: qsTr("A simple text editor for the Nokia N9.") + "\n\n"+
                 AppDefaults.HOMEPAGE+"\n"+
                 qsTr("License")+": GPL3\n"+
                 qsTr("Contact")+": <n9dyfi@gmail.com>\n";
        acceptButtonText: "Go to homepage"
        rejectButtonText: "Close"
        onAccepted: {
            Qt.openUrlExternally(AppDefaults.HOMEPAGE)
        }
    }
}
