import QtQuick 1.1
import com.nokia.meego 1.0

// Toolbar for the Edit page
ToolBar {
    anchors.bottom: parent.bottom
    tools:
        ToolBarLayout {
        // Quick save button
        ToolIcon {
            platformIconId: "toolbar-directory-move-to"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: {
                menuSaveClicked(editPage.content)
            }
        }
        // Menu button
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (editMenu.status === DialogStatus.Closed) ?
                           editMenu.open() : editMenu.close()
        }
    }
}
