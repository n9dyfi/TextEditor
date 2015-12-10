import QtQuick 1.1
import com.nokia.meego 1.0
import "appDefaults.js" as AppDefaults


Page {

    property alias content: textArea.text
    property string currentFolder: ""
    property string currentFile: ""
    property bool save: false
    property alias textAreaHeight: textArea.height

    tools: commonTools

    // Instantiate the Tools component (defined in EditTools.qml)
    EditTools{
        id: commonTools
    }

    Header {
        id: header
        headerText: "TextEditor"
        singleLineHeader: (currentFile=="")
        infoTopText: currentFile
        infoBottomText: currentFolder
    }

    Flickable {
        id: flickable
        anchors.top: header.bottom
        width: parent.width
        height: parent.height-header.height
        clip: true
        contentHeight: textArea.height
        contentWidth: textArea.width

        TextArea {
            id: textArea
            width: editPage.width
            placeholderText: AppDefaults.PLACE_HOLDER_TEXT
            smooth: true
            wrapMode: TextEdit.Wrap
            textFormat: TextEdit.PlainText
            font.pixelSize: AppDefaults.FONT_SIZE_SMALL
        }
    }

    // Called twice: 1) save=true 2) save=false
    // Change state from "saving" to "saved"
    onSaveChanged: {
        state = (save)?"saving":"saved"
        save = false
    }

    state: "saved"
    states: [
        State {
            name: "saving"
            PropertyChanges {target: statusLabel; opacity: 1.0}
            PropertyChanges {target: flickable; opacity: 0}
        },
        State {
            name: "saved"
        }
    ]

    // Status label to give some visual feedback for the "save" button click
    Label {
        id: statusLabel
        opacity: 0
        color:  "orange"
        font.pixelSize: 42
        text: "Saving..."
        anchors.centerIn: parent
    }

    // Animations to give some visual feedback for the "save" button click
    transitions: [
        Transition {
            from: "saving"
            to: "saved"
            PropertyAnimation {
                target: statusLabel
                property: "opacity"
                to: 0
                duration: 1000
            }
            PropertyAnimation {
                target: flickable
                property: "opacity"
                to: 1
                duration: 1000
            }
        }
    ]

}
