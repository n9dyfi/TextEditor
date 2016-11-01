import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {

    id: appWindow

    // UI constants
    property int defaultMargin : appDefaults.cDEFAULT_MARGIN
    property bool orientationIsPortrait

    // Select the color scheme before instantiating any QML elements
    // that need the color...
    onDefaultMarginChanged: theme.colorScheme = appDefaults.cCOLOR_SCHEME

    showStatusBar: false;

    property alias content: editPage.content
    property bool appIsClosing: false

     // These QML signals will be used in the QML side only
    signal browseCancelled
    signal menuAboutClicked

    // These QML signals will be connected to the corresponding TextEditor slots
    signal menuNewClicked(string content)
    signal menuOpenClicked(string content)
    signal menuSaveAsClicked()
    signal menuQuitClicked(string content)
    signal toolSaveClicked(string content)
    signal toolRecentClicked()
    signal newFolderChanged(string newFolder)
    signal saveAsRequested(string content, string fileName)
    signal fileOpenRequested(string fileName)
    signal saveAsConfirmed(string content)
    signal newOrOpenConfirmed(string op)
    signal openRecentConfirmed()
    signal saveBeforeClosed(string content)
    signal recentFileClicked(string fileName, string folderName, string content)
    signal clearRecentClicked()

    // The corresponding TextEditor signals will be connected to these QML signals
    signal browseRequested(string currentFolder, bool saveRequested)
    signal recentRequested()
    signal openCompleted(string content,string currentFolder, string currentFile)
    signal openFailed(string fileName, string errorString)
    signal saveCompleted
    signal saveFailed(string fileName, string errorString)
    signal saveAsCompleted(string currentFolder, string currentFile)
    signal saveAsToBeConfirmed(string fileName)
    signal newOrOpenToBeConfirmed(string op, string fileName)
    signal openRecentToBeConfirmed(string fileName)
    signal appCloseToBeConfirmed(string fileName)
    signal appToBeClosed
    signal editorCleared(string folderPath,string fileName)

    // Instantiate the EditPage component (defined in EditPage.qml)
    initialPage: editPage
    EditPage {
        id: editPage
    }

    // Application defaults, visible in all pages
    AppDefaults {
        id: appDefaults
    }

    // QML component loader
    Loader {
        id: myLoader
    }

    // Event handlers

    onEditorCleared: {
        editPage.content = ""
        editPage.currentFile = fileName
        editPage.currentFolder = folderPath
        editPage.textAreaHeight = appDefaults.cTEXT_AREA_HEIGHT
    }

    // Common exit point when Menu>Quit was selected
    onAppToBeClosed: {
        Qt.quit()
    }

    // Menu>Cancel was selected in the BrowsePage
    onBrowseCancelled: {
        pageStack.pop()
    }

    // TextEditor requested BrowsePage to be opened for selecting a file.
    onBrowseRequested: {
        pageStack.push(Qt.resolvedUrl("BrowsePage.qml"),
                       {folderPath:currentFolder,saveAs:saveRequested});
    }

    // TextEditor requested RecentPage to be opened for selecting a file.
    onRecentRequested: {
        pageStack.push(Qt.resolvedUrl("RecentPage.qml"))
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

    // Dialogs

    // Menu>About was clicked
    onMenuAboutClicked: {
        myLoader.source = "DialogAbout.qml"
        myLoader.item.show()
    }

    // Menu>Quit was selected but editor contents were not saved.
    onAppCloseToBeConfirmed: {
        myLoader.source = "DialogConfirmQuit.qml"
        myLoader.item.title = qsTr("%1 changed.").arg(fileName)
        myLoader.item.show()
    }

    // TextEditor could not open a file for reading.
    onOpenFailed: {
        myLoader.source = "DialogFileError.qml"
        myLoader.item.title = qsTr("Cannot open %1.").arg(fileName)
        myLoader.item.message = errorString;
        myLoader.item.show()
    }

    // TextEditor could not save the file.
    onSaveFailed: {
        myLoader.source = "DialogFileError.qml"
        myLoader.item.title = qsTr("Cannot save %1.").arg(fileName)
        myLoader.item.message = errorString;
        myLoader.item.show();
    }

    // TextEditor will ask before overwriting an existing file.
    onSaveAsToBeConfirmed: {
        myLoader.source = "DialogConfirmSave.qml"
        myLoader.item.title = qsTr("%1 already exists.").arg(fileName)
        myLoader.item.show()
    }

    // TextEditor will ask before opening a new file when editor contents changed.
    onNewOrOpenToBeConfirmed: {
        myLoader.source = "DialogConfirmNewOrOpen.qml"
        myLoader.item.title = qsTr("%1 changed.").arg(fileName)
        myLoader.item.show(op)
    }

    onOpenRecentToBeConfirmed: {
        myLoader.source = "DialogConfirmOpenRecent.qml"
        myLoader.item.title = qsTr("%1 changed.").arg(fileName)
        myLoader.item.show()
    }

}
