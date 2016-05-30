#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QDeclarativeItem>
#include <QFile>
//#include <QDesktopServices>
#include "recentfiles.h"
#include "qmlapplicationviewer.h"

#define SAVE 0
#define SAVE_AS 1

class TextEditor : public QObject
{
    Q_OBJECT

public:
    explicit TextEditor(QObject *qml, RecentFiles *recentfiles, QObject *parent = 0);

private:
    static const char *UNTITLED;    // default file name for File>New

    QString currentFolder;
    QString currentFile;
    QString currentContent;

    // File info for open/save as, copied to current if operation is succesfull

    QString newFolder;
    QString newFile;

    RecentFiles *recentFiles;

private slots:
    void saveCurrentContent(int);

public slots:
    void menuNewClicked(QString);
    void menuOpenClicked(QString);
    void menuSaveAsClicked();
    void menuQuitClicked(QString);
    void toolSaveClicked(QString);
    void toolRecentClicked(QString);
    void saveAsRequested(QString,QString);
    void newFolderChanged(QString);
    void fileOpenRequested(QString);
    void saveAsConfirmed(QString);
    void newOrOpenConfirmed(QString);
    void openRecentConfirmed();
    void saveBeforeClosed(QString);
    void recentFileClicked(QString,QString,QString);

signals:
    void browseRequested(QString currentFolder, bool saveRequested);
    void recentRequested();
    void openCompleted(QString content, QString currentFolder, QString currentFile);
    void openFailed(QString fileName, QString errorString);
    void saveCompleted();
    void saveFailed(QString fileName, QString errorString);
    void saveAsCompleted(QString currentFolder, QString currentFile);
    void saveAsToBeConfirmed(QString fileName);
    void newOrOpenToBeConfirmed(QString op, QString fileName);
    void openRecentToBeConfirmed(QString fileName);
    void appCloseToBeConfirmed(QString fileName);
    void appToBeClosed();
    void editorCleared(QString currentFolder, QString currentFile);
};

#endif // TEXTEDITOR_H
