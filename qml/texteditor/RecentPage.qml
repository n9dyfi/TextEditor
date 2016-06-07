import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1     // set QML_IMPORT_PATH...
import "appDefaults.js" as AppDefaults

Page {

    signal recentCancelled

    // Toolbar>Back was clicked in the RecentPage
    onRecentCancelled: {
        pageStack.pop()
    }

    // Instantiate the RecentTools component (defined in RecentTools.qml)
    RecentTools{
        id: recentTools
        visible: true
    }

    // The page header
    Header {
        id: header
        anchors.top: parent.top
        singleLineHeader: true
        headerText: qsTr("Recent Files")
    }

    // Recent files list
    ListView {
        id: listView
        model: recentfiles  // from the root context
        delegate: ListDelegate {
            onClicked: {
                recentFileClicked(model.title,model.subtitle,editPage.content)
            }
        }
        anchors.top: header.bottom
        x: AppDefaults.DEFAULT_MARGIN
        height: parent.height - header.height - AppDefaults.DEFAULT_MARGIN - recentTools.height;
        width: parent.width;
        clip: true
    }

}
