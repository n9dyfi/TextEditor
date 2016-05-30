#include "texteditor.h"
#include "recentfiles.h"
//#include <QDebug>

// QT_TRANSLATE_NOOP is required to translate static const strings outside any context
const char *TextEditor::UNTITLED = QT_TRANSLATE_NOOP("TextEditor","Untitled");

TextEditor::TextEditor(QObject *qml, RecentFiles *recentfiles, QObject *parent) :
    QObject(parent)
{

    currentFolder = "file:///home/user";
    newFolder = currentFolder;
    //currentFolder = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
    //qDebug() << currentFolder;
    currentFile = tr(UNTITLED);
    currentContent = "";
    recentFiles = recentfiles;

    // connect QML signals to TextEditor slots
    connect(qml, SIGNAL(menuOpenClicked(QString)),
             this, SLOT(menuOpenClicked(QString)));
    connect(qml, SIGNAL(toolSaveClicked(QString)),
             this, SLOT(toolSaveClicked(QString)));
    connect(qml, SIGNAL(toolRecentClicked(QString)),
             this, SLOT(toolRecentClicked(QString)));
    connect(qml, SIGNAL(menuSaveAsClicked()),
             this, SLOT(menuSaveAsClicked()));
    connect(qml, SIGNAL(saveAsRequested(QString,QString)),
             this, SLOT(saveAsRequested(QString,QString)));
    connect(qml, SIGNAL(newFolderChanged(QString)),
             this, SLOT(newFolderChanged(QString)));
    connect(qml, SIGNAL(fileOpenRequested(QString)),
             this, SLOT(fileOpenRequested(QString)));
    connect(qml, SIGNAL(saveAsConfirmed(QString)),
             this, SLOT(saveAsConfirmed(QString)));
    connect(qml, SIGNAL(newOrOpenConfirmed(QString)),
             this, SLOT(newOrOpenConfirmed(QString)));
    connect(qml, SIGNAL(openRecentConfirmed()),
             this, SLOT(openRecentConfirmed()));
    connect(qml, SIGNAL(menuQuitClicked(QString)),
             this, SLOT(menuQuitClicked(QString)));
    connect(qml, SIGNAL(saveBeforeClosed(QString)),
             this, SLOT(saveBeforeClosed(QString)));
    connect(qml, SIGNAL(menuNewClicked(QString)),
             this, SLOT(menuNewClicked(QString)));
    connect(qml, SIGNAL(recentFileClicked(QString,QString,QString)),
             this, SLOT(recentFileClicked(QString,QString,QString)));

    // connect TextEditor signals to QML signals
    connect(this, SIGNAL(browseRequested(QString,bool)),
               qml, SLOT(browseRequested(QString,bool)));
    connect(this, SIGNAL(recentRequested()),
               qml, SLOT(recentRequested()));
    connect(this, SIGNAL(openCompleted(QString,QString,QString)),
               qml, SLOT(openCompleted(QString,QString,QString)));
    connect(this, SIGNAL(openFailed(QString,QString)),
               qml, SLOT(openFailed(QString,QString)));
    connect(this, SIGNAL(saveCompleted()),
               qml, SLOT(saveCompleted()));
    connect(this, SIGNAL(saveFailed(QString,QString)),
               qml, SLOT(saveFailed(QString,QString)));
    connect(this, SIGNAL(saveAsCompleted(QString,QString)),
               qml, SLOT(saveAsCompleted(QString,QString)));
    connect(this, SIGNAL(saveAsToBeConfirmed(QString)),
               qml, SLOT(saveAsToBeConfirmed(QString)));
    connect(this, SIGNAL(newOrOpenToBeConfirmed(QString,QString)),
               qml, SLOT(newOrOpenToBeConfirmed(QString,QString)));
    connect(this, SIGNAL(openRecentToBeConfirmed(QString)),
               qml, SLOT(openRecentToBeConfirmed(QString)));
    connect(this, SIGNAL(appCloseToBeConfirmed(QString)),
               qml, SLOT(appCloseToBeConfirmed(QString)));
    connect(this, SIGNAL(appToBeClosed()),
               qml, SLOT(appToBeClosed()));
    connect(this, SIGNAL(editorCleared(QString,QString)),
               qml, SLOT(editorCleared(QString,QString)));
}

// QML menu: Open was clicked
void TextEditor::menuOpenClicked(QString content)
{
    if(content!=currentContent)
    {
        emit newOrOpenToBeConfirmed("open",currentFile);
        return;
    }
    bool saveRequested = false;
    emit browseRequested(currentFolder, saveRequested);
}

// QML Toolbar>New clicked
void TextEditor::menuNewClicked(QString content)
{
    if(content!=currentContent)
    {
        emit newOrOpenToBeConfirmed("new",currentFile);
        return;
    }
    newOrOpenConfirmed("new");
}

// QML menu: Save was clicked
void TextEditor::toolSaveClicked(QString content)
{
    if(currentFile==tr(UNTITLED))
    {
        menuSaveAsClicked();
        return;
    }
    currentContent = content;
    newFolder = currentFolder;
    newFile = currentFile;
    saveCurrentContent(SAVE);
}

// EditPage toolbar: Recent Files was clicked
void TextEditor::toolRecentClicked(QString content)
{
    bool statusOk = recentFiles->readRecentFiles();
    if (statusOk)
        emit recentRequested();
}

// QML menu: Save As was clicked
void TextEditor::menuSaveAsClicked()
{
    bool saveRequested = true;
    emit browseRequested(currentFolder, saveRequested);
}

// Overwrite during Menu>Save As was confirmed.
void TextEditor::saveAsConfirmed(QString content)
{
    currentContent = content;
    saveCurrentContent(SAVE_AS);
}

// Discard changes and proceed with "New" or "Open"
void TextEditor::newOrOpenConfirmed(QString op)
{
    if(op=="new")
    {
        currentFile = tr(UNTITLED);
        currentContent = "";
        emit editorCleared(currentFolder,currentFile);
    } else if(op=="open") {
        bool saveRequested = false;
        emit browseRequested(currentFolder, saveRequested);
    }
}

// Discard changes and proceed with "Open recent"
void TextEditor::openRecentConfirmed()
{
    fileOpenRequested(newFile);
}

// Menu>Quit was selected.
void TextEditor::menuQuitClicked(QString content)
{
    // Check if the content was changed.
    if(content!=currentContent)
    {
        emit appCloseToBeConfirmed(currentFile);
        return;
    }
    // Not changed -> ok to quit.
    emit appToBeClosed();
}

// Saving changed content before quitting was confirmed.
void TextEditor::saveBeforeClosed(QString content)
{
    // Did we have a valid file name?
    if(currentFile==tr(UNTITLED))
    {
        menuSaveAsClicked();
        return;
    }
    currentContent = content;
    newFolder = currentFolder;
    newFile = currentFile;
    saveCurrentContent(SAVE);
}

void TextEditor::recentFileClicked(QString fileName, QString folderName, QString content)
{
    newFolder = folderName;
    newFile = fileName;
    // Check if the content was changed.
    if(content!=currentContent)
    {
        emit openRecentToBeConfirmed(currentFile);
        return;
    }
    // Not changed -> ok to open.
    fileOpenRequested(newFile);
}

// Save the current editor content to the current folder/file.
void TextEditor::saveCurrentContent(int saveMode)
{
    bool fileIsWritable;

    //qDebug() << "DBG> save"<<currentFile<<"to currentFolder:"<<currentFolder;

    QUrl url(newFolder+"/"+newFile);
    QString localFile = url.toLocalFile();
    QFile file(localFile);
    fileIsWritable = file.open(QIODevice::WriteOnly | QIODevice::Text);

    if ( fileIsWritable ) {
        QTextStream stream( &file );
        stream<<currentContent;
        file.close();
        currentFolder = newFolder;
        currentFile = newFile;
        if(saveMode==SAVE)
            emit saveCompleted();
        else {
            recentFiles->readRecentFiles();
            recentFiles->addFile(currentFolder+"|"+currentFile);
            emit saveAsCompleted(currentFolder,currentFile);
        }
    } else {
        emit saveFailed(newFile,file.errorString());
    }
}

// QML listView: file name was selected for saving
void TextEditor::saveAsRequested(QString content, QString fileName)
{
    if (fileName.isEmpty())
        return;

    newFile = fileName;
    QUrl url(newFolder+"/"+newFile);
    QString localFile = url.toLocalFile();
    QFile file(localFile);
    if(file.exists())
    {
        emit saveAsToBeConfirmed(fileName);
    } else {
        currentContent = content;
        saveCurrentContent(SAVE_AS);
    }
}

// QML listView: folder was selected
void TextEditor::newFolderChanged(QString path)
{
    newFolder = path;
}

// QML listView: file name was selected for reading
void TextEditor::fileOpenRequested(QString fileName)
{
    bool fileIsReadable = false;

    //qDebug() << "DBG> open"<<fileName<<"from currentFolder:"<<currentFolder;

    QUrl url(newFolder+"/"+fileName);
    QString localFile = url.toLocalFile();
    QFile file(localFile);
    fileIsReadable = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (fileIsReadable)
    {
        QTextStream stream( &file );
        currentContent = stream.readAll();
        file.close();
        // update current folder and file name
        currentFolder = newFolder;
        currentFile = fileName;
        // update the recent files list
        recentFiles->readRecentFiles();
        recentFiles->addFile(currentFolder+"|"+currentFile);
        // pass the content to the TextArea component
        emit openCompleted(currentContent,currentFolder,currentFile);
    } else {
        // remove the entry from the recent files list (if it exists there)
        // do not touch the current folder and file names
        recentFiles->removeFile(newFolder+"|"+fileName);
        emit openFailed(fileName,file.errorString());
    }
}
