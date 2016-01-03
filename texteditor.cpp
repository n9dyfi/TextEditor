#include "texteditor.h"

const char *TextEditor::UNTITLED = "Untitled";

TextEditor::TextEditor(QObject *qml, QObject *parent) :
      QObject(parent)
{
    currentFolder = "file:///home/user";
    currentFile = UNTITLED;
    currentContent = "";

    // connect QML signals to TextEditor slots
    connect(qml, SIGNAL(menuOpenClicked(QString)),
             this, SLOT(menuOpenClicked(QString)));
    connect(qml, SIGNAL(menuSaveClicked(QString)),
             this, SLOT(menuSaveClicked(QString)));
    connect(qml, SIGNAL(menuSaveAsClicked()),
             this, SLOT(menuSaveAsClicked()));
    connect(qml, SIGNAL(saveAsRequested(QString,QString)),
             this, SLOT(saveAsRequested(QString,QString)));
    connect(qml, SIGNAL(currentFolderChanged(QString)),
             this, SLOT(currentFolderChanged(QString)));
    connect(qml, SIGNAL(fileOpenRequested(QString)),
             this, SLOT(fileOpenRequested(QString)));
    connect(qml, SIGNAL(saveAsConfirmed(QString)),
             this, SLOT(saveAsConfirmed(QString)));
    connect(qml, SIGNAL(openConfirmed()),
             this, SLOT(openConfirmed()));
    connect(qml, SIGNAL(newConfirmed()),
             this, SLOT(newConfirmed()));
    connect(qml, SIGNAL(appCloseRequested(QString)),
             this, SLOT(appCloseRequested(QString)));
    connect(qml, SIGNAL(saveBeforeClosed(QString)),
             this, SLOT(saveBeforeClosed(QString)));
    connect(qml, SIGNAL(fileNewRequested(QString)),
             this, SLOT(fileNewRequested(QString)));

    // connect TextEditor signals to QML signals
    connect(this, SIGNAL(browseRequested(QString,bool)),
               qml, SLOT(browseRequested(QString,bool)));
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
    connect(this, SIGNAL(openToBeConfirmed(QString)),
               qml, SLOT(openToBeConfirmed(QString)));
    connect(this, SIGNAL(newToBeConfirmed(QString)),
               qml, SLOT(newToBeConfirmed(QString)));
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
        emit openToBeConfirmed(currentFile);
        return;
    }
    bool saveRequested = false;
    emit browseRequested(currentFolder, saveRequested);
}

// QML Toolbar>New clicked
void TextEditor::fileNewRequested(QString content)
{
    if(content!=currentContent)
    {
        emit newToBeConfirmed(currentFile);
        return;
    }
    newConfirmed();
}

// QML menu: Save was clicked
void TextEditor::menuSaveClicked(QString content)
{
    if(currentFile==UNTITLED)
    {
        menuSaveAsClicked();
        return;
    }
    currentContent = content;
    saveCurrentContent(SAVE);
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

// Discard current content was confirmed during Menu>Open.
void TextEditor::openConfirmed()
{
    bool saveRequested = false;
    emit browseRequested(currentFolder, saveRequested);
}

// Discard current content was confirmed during Menu>New.
void TextEditor::newConfirmed()
{
    currentFile = UNTITLED;
    currentContent = "";
    emit editorCleared(currentFolder,currentFile);
}

// Menu>Quit was selected.
void TextEditor::appCloseRequested(QString content)
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
    if(currentFile==UNTITLED)
    {
        menuSaveAsClicked();
        return;
    }
    currentContent = content;
    saveCurrentContent(SAVE);
}

// Save the current editor content to the current folder/file.
void TextEditor::saveCurrentContent(int saveMode)
{
    bool fileIsWritable;

    //qDebug() << "DBG> save"<<currentFile<<"to currentFolder:"<<currentFolder;

    QUrl url(currentFolder+"/"+currentFile);
    QString filename = url.toLocalFile();
    QFile file(filename);
    fileIsWritable = file.open(QIODevice::WriteOnly | QIODevice::Text);

    if ( fileIsWritable ) {
        QTextStream stream( &file );
        stream<<currentContent;
        file.close();
        if(saveMode==SAVE)
            emit saveCompleted();
        else
            emit saveAsCompleted(currentFolder,currentFile);
    } else {
        emit saveFailed(currentFile,file.errorString());
    }
}

// QML listView: file name was selected for saving
void TextEditor::saveAsRequested(QString content, QString fileName)
{
    currentFile = fileName;

    QUrl url(currentFolder+"/"+currentFile);
    QString filename = url.toLocalFile();
    QFile file(filename);
    if(file.exists())
    {
        emit saveAsToBeConfirmed(currentFile);
    } else {
        currentContent = content;
        saveCurrentContent(SAVE_AS);
    }
}

// QML listView: folder was selected
void TextEditor::currentFolderChanged(QString path)
{
    currentFolder = path;
}

// QML listView: file name was selected for reading
void TextEditor::fileOpenRequested(QString fileName)
{
    bool fileIsReadable = false;

    //qDebug() << "DBG> open"<<fileName<<"from currentFolder:"<<currentFolder;

    QUrl url(currentFolder+"/"+fileName);
    QString localFile = url.toLocalFile();
    QFile file(localFile);
    fileIsReadable = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (fileIsReadable)
    {
        QTextStream stream( &file );
        currentContent = stream.readAll();
        file.close();
        // pass the content to the TextArea component
        currentFile = fileName;
        emit openCompleted(currentContent,currentFolder,currentFile);
    } else {
        emit openFailed(fileName,file.errorString());
    }
}
