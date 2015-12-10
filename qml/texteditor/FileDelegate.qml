import QtQuick 1.1
import "appDefaults.js" as AppDefaults
Rectangle {
    id: fileDelegate
    property bool isDir: false

    width: parent.width
    height: fileNameView.height * 1.5
    color: ( mouseArea.pressed )
           ? AppDefaults.FILE_BG_COLOR_PRESSED
           : AppDefaults.FILE_BG_COLOR_NORMAL
    Image
    {
        id: icon
        anchors.verticalCenter: parent.verticalCenter
        x: AppDefaults.DEFAULT_MARGIN
        smooth: true
        source: isDir
                ? "image://theme/icon-m-toolbar-directory-white"
                : "image://theme/icon-s-notes";
        visible: source != ''
    }

    Text {
        id: fileNameView
        text: fileName
        font.pixelSize: AppDefaults.FONT_SIZE_SMALL
        anchors.verticalCenter: parent.verticalCenter
        anchors.left:icon.right
        anchors.leftMargin: AppDefaults.DEFAULT_MARGIN
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
            fileNameView.color="red"
            if ( isDir ) {
                folderChanged(filePath);
            } else {
                if (saveAs) {
                    saveasfile.text = fileName;
                } else {
                    fileOpenRequested(fileName)
                }
            }
        }
    }
}
