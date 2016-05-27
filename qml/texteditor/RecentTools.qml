import QtQuick 1.1
import com.nokia.meego 1.0

// Toolbar for the recent files page
ToolBar {
    anchors.bottom: parent.bottom
    tools:
        ToolBarLayout {
        // Back to the EditPage
        ToolIcon {
            platformIconId: inPortrait?"toolbar-back-dimmed-white":
                                        "toolbar-back-landscape-dimmed-white"
            onClicked: recentCancelled()
        }
    }
}
