import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    signal show
    onShow: dialog.open()
    QueryDialog {
        id: dialog
        titleText: "TextEditor "+appDefaults.cVERSION
        message: qsTr("A simple text editor for the Nokia N9.") + "\n\n"+
                 appDefaults.cHOMEPAGE+"\n"+
                 qsTr("License")+": GPL3\n"+
                 qsTr("Contact")+": <n9dyfi@gmail.com>\n";
        acceptButtonText: qsTr("Go to homepage")
        rejectButtonText: qsTr("Close")
        onAccepted: {
            Qt.openUrlExternally(appDefaults.cHOMEPAGE)
        }
    }
}
