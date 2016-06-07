import QtQuick 1.1

Rectangle {
    id: fileDelegate
    property bool isDir: false

    width: parent.width
    height: fileNameView.height * 1.5
    color: ( mouseArea.pressed )
           ? appDefaults.cFILE_BG_COLOR_PRESSED
           : appDefaults.cFILE_BG_COLOR_NORMAL
    Image
    {
        id: icon
        anchors.verticalCenter: parent.verticalCenter
        x: appDefaults.cDEFAULT_MARGIN
        smooth: true
        source: isDir
                ? "image://theme/icon-m-toolbar-directory-white"
                : "image://theme/icon-s-notes";
        visible: source != ''
    }

    Text {
        id: fileNameView
        text: fileName
        font.pixelSize: appDefaults.cFONT_SIZE_SMALL
        anchors.verticalCenter: parent.verticalCenter
        anchors.left:icon.right
        anchors.leftMargin: appDefaults.cDEFAULT_MARGIN
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
