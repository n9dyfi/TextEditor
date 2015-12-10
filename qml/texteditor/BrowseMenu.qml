import QtQuick 1.1
import com.nokia.meego 1.0

Menu {
    MenuLayout {
        MenuItem {
            text: qsTr("Cancel")
            onClicked: browseCancelled()
        }
    }
}
