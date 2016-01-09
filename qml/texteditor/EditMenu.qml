import QtQuick 1.1
import com.nokia.meego 1.0

Menu {

    MenuLayout {

        MenuItem {
            text: "New"
            onClicked: {
                fileNewRequested(editPage.content)
            }
        }

        MenuItem {
            text: "Open"
            onClicked: {
                menuOpenClicked(editPage.content)
            }
        }

        MenuItem {
            text: "Save As"
            onClicked: {
                menuSaveAsClicked()
            }
        }

        MenuItem {
            text: "About"
            onClicked: {
                aboutDialog.open()
            }
        }

        MenuItem {
            text: "Quit"
            onClicked: {
                appCloseRequested(editPage.content)
            }
        }
    }
}
