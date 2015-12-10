import QtQuick 1.1
import com.nokia.meego 1.0

// Toolbar for the browse page
ToolBar {
    anchors.bottom: parent.bottom
    tools:
        ToolBarLayout {
        // Back to the parent folder
        ToolIcon {
            platformIconId: inPortrait?"toolbar-back-dimmed-white":
                                        "toolbar-back-landscape-dimmed-white"
            onClicked: folderChanged(folderModel.parentFolder);
        }
        // Refresh
        ToolIcon {
            platformIconId: "toolbar-refresh"
            onClicked: {
                refresh=true
            }
        }
        // Menu
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (browseMenu.status === DialogStatus.Closed) ?
                           browseMenu.open() :
                           browseMenu.close()
        }
    }
}
