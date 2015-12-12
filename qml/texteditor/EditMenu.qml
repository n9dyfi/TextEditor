import QtQuick 1.1
import com.nokia.meego 1.0

Menu {

    MenuLayout {

        MenuItem {
            text: qsTr("New")
            onClicked: {
                fileNewRequested(editPage.content)
            }
        }

        MenuItem {
            text: qsTr("Open")
            onClicked: {
                menuOpenClicked(editPage.content)
            }
        }

        MenuItem {
            text: qsTr("Save As")
            onClicked: {
                menuSaveAsClicked()
            }
        }

        MenuItem {
            text: qsTr("About")
            onClicked: {
                aboutDialog.open();
            }
        }

        MenuItem {
            text: qsTr("Quit")
            onClicked: {
                appCloseRequested(editPage.content);
            }
        }
    }
}
