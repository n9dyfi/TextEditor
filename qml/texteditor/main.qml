import QtQuick 1.1
import com.nokia.meego 1.0
import "appDefaults.js" as AppDefaults

PageStackWindow {

    id: appWindow

    // UI constants
    property int defaultMargin : AppDefaults.DEFAULT_MARGIN
    property bool orientationIsPortrait
    property string aboutInfo: qsTr("A simple text editor for the Nokia N9.")+"\n\n"+
                               AppDefaults.HOMEPAGE+"\n"+
                               qsTr("License")+": GPL3\n"+
                               qsTr("Contact")+": <n9dyfi@gmail.com>\n";

    // Select the color scheme before instantiating any QML elements
    // that need the color...
    onDefaultMarginChanged: theme.colorScheme = AppDefaults.COLOR_SCHEME

    showStatusBar: false;

    property alias content: editPage.content
    //property alias folderPath: browsePage.folderPath

    property bool appIsClosing: false
    signal browseCancelled

    // These QML signals will be connected to the corresponding TextEditor slots
    signal menuOpenClicked(string content)
    signal menuSaveClicked(string content)
    signal menuSaveAsClicked()
    signal currentFolderChanged(string newFolder)
    signal saveAsRequested(string content, string fileName)
    signal fileOpenRequested(string fileName)
    signal fileNewRequested(string content)
    signal saveAsConfirmed(string content)
    signal openConfirmed
    signal newConfirmed
    signal appCloseRequested(string content)
    signal saveBeforeClosed(string content)

    // The corresponding TextEditor signals will be connected to these QML signals
    signal browseRequested(string currentFolder, bool saveRequested)
    signal openCompleted(string content,string currentFolder, string currentFile)
    signal openFailed(string fileName, string errorString)
    signal saveCompleted(string currentFolder, string currentFile)
    signal saveFailed(string fileName, string errorString)
    signal saveAsCompleted(string currentFolder, string currentFile)
    signal saveAsToBeConfirmed(string fileName)
    signal openToBeConfirmed(string fileName)
    signal newToBeConfirmed(string fileName)
    signal appCloseToBeConfirmed(string fileName)
    signal appToBeClosed
    signal editorCleared(string folderPath,string fileName)

    // Instantiate the EditPage component (defined in EditPage.qml)
    initialPage: editPage
    EditPage {
        id: editPage
    }

    // Instantiate the BrowsePage component (defined in BrowsePage.qml)
    BrowsePage {
        id: browsePage;
    }

    // Instantiate the EditMenu component (defined in EditMenu.qml)
    EditMenu {
        id: editMenu
    }

    // Instantiate the BrowseMenu component (defined in BrowseMenu.qml)
    BrowseMenu {
        id: browseMenu
    }

    // Event handlers

    onEditorCleared: {
        editPage.content = ""
        editPage.currentFile = fileName
        editPage.currentFolder = folderPath
        editPage.textAreaHeight = AppDefaults.TEXT_AREA_HEIGHT
    }

    // Common exit point when Menu>Quit was selected
    onAppToBeClosed: {
        Qt.quit()
    }

    // Menu>Cancel was selected in the BrowsePage
    onBrowseCancelled: {
        pageStack.pop()
    }

    // Menu>Quit was selected but editor contents were not saved.
    onAppCloseToBeConfirmed: {
        //appCloseConfirmDialog.titleText = fileName+" "+qsTr("changed.")
        appCloseConfirmDialog.titleText = qsTr("%1 changed.").arg(fileName)
        appCloseConfirmDialog.open();
    }    

    // TextEditor requested BrowsePage to be opened for selecting a file.
    onBrowseRequested: {
        pageStack.push(browsePage,{folderPath:currentFolder,saveAs:saveRequested});
    }

    // TextEditor could not open a file for reading.
    onOpenFailed: {
        openFailedDialog.titleText = qsTr("Cannot open %1.").arg(fileName)
        openFailedDialog.message = errorString;
        openFailedDialog.open();
    }

    // TextEditor could not save the file.
    onSaveFailed: {
        saveFailedDialog.titleText = qsTr("Cannot save %1.").arg(fileName)
        saveFailedDialog.message = errorString;
        saveFailedDialog.open();
    }

    // TextEditor successfully saved the editor contents to the selected file.
    // Go back to EditPage and flash "Saving...".
    onSaveAsCompleted: {
        editPage.currentFolder = currentFolder;
        editPage.currentFile = currentFile;
        pageStack.pop();
        editPage.showSave = true
        // Was this the save as before close?
        if(appIsClosing) {
            appToBeClosed()
        }
    }

    // TextEditor successfully saved the editor contents to the current file.
    // Stay in EditPage and flash "Saving...".
    onSaveCompleted: {
        editPage.showSave = true
        // Was this the save before close?
        if(appIsClosing) {
            appToBeClosed()
        }
    }

    // TextEditor successfully loaded the editor contents from the selected file.
    onOpenCompleted: {
        editPage.content = content;
        editPage.currentFolder = currentFolder;
        editPage.currentFile = currentFile;
        pageStack.pop();
    }

    // TextEditor will ask before overwriting an existing file.
    onSaveAsToBeConfirmed: {
        saveConfirmDialog.titleText = qsTr("%1 already exists.").arg(fileName)
        saveConfirmDialog.open();
    }

    // TextEditor will ask before opening a new file when editor contents changed.
    onOpenToBeConfirmed: {
        openConfirmDialog.titleText = qsTr("%1 changed.").arg(fileName)
        openConfirmDialog.open();
    }

    // TextEditor will ask before starting a new file when editor contents changed.
    onNewToBeConfirmed: {
        newConfirmDialog.titleText = qsTr("%1 changed.").arg(fileName)
        newConfirmDialog.open();
    }

    // Dialogs

    // About TextEditor
    QueryDialog {
        id: aboutDialog
        titleText: "TextEditor "+AppDefaults.VERSION
        message: aboutInfo
        acceptButtonText: qsTr("Go to homepage")
        rejectButtonText: qsTr("Close")
        onAccepted: {
            Qt.openUrlExternally(AppDefaults.HOMEPAGE)
        }
    }

    // File open error
    QueryDialog {
        id: openFailedDialog
        acceptButtonText: qsTr("OK")
    }

    // File save error
    QueryDialog {
        id: saveFailedDialog
        acceptButtonText: qsTr("OK")
    }

    // Overwrite confirmation
    QueryDialog {
        id: saveConfirmDialog
        message: qsTr("Do you want to replace it?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        onAccepted: {
            saveAsConfirmed(editPage.content)
        }
    }

    // Overwrite editor contents confirmation
    QueryDialog {
        id: appCloseConfirmDialog
        message: qsTr("Save before exiting?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        onAccepted: {
            appIsClosing = true
            saveBeforeClosed(editPage.content)
        }
        onRejected: {
            appToBeClosed()
        }
    }

    // Overwrite editor contents confirmation
    QueryDialog {
        id: openConfirmDialog
        message: qsTr("Discard changes?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        onAccepted: {
            openConfirmed()
        }
    }

    // Overwrite editor contents confirmation
    QueryDialog {
        id: newConfirmDialog
        message: qsTr("Discard changes?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        onAccepted: {
            newConfirmed()
        }
    }
}
