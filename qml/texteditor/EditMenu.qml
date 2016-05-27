import QtQuick 1.1
import com.nokia.meego 1.0

Menu {

    MenuLayout {

        MenuItem {
            text: qsTr("New")
            onClicked: {
                menuNewClicked(editPage.content)
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
                menuAboutClicked();
            }
        }

        MenuItem {
            text: qsTr("Quit")
            onClicked: {
                menuQuitClicked(editPage.content);
            }
        }
    }
}
