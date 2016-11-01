#include <QtGui/QApplication>
#include <QDir>
#include "recentfiles.h"

// Folder and filename for saving the recent files list
const char *RecentFiles::CONFIG_FOLDER = ".texteditor";
const char *RecentFiles::RECENT_FILES = "recentfiles.dat";

RecentFiles::RecentFiles(QObject *qml, QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[FolderRole] = "subtitle";
    roles[FileRole] = "title";
    // expose the roles to the QML side so that the ListDelegate can refer
    // to role names "subtitle" and "title".
    setRoleNames(roles);

    recentFilesLoaded = false;
    recentFilesModified = false;

    // Display and error message if we cannot open the file
    // containing the recent files list
    connect(this, SIGNAL(openFailed(QString,QString)),
               qml, SIGNAL(openFailed(QString,QString)));

    // The delete button was clicked
    connect(qml, SIGNAL(clearRecentClicked()), this, SLOT(clearFiles()));

    // Catch application exit to save the recent files list
    connect(QCoreApplication::instance(),SIGNAL(aboutToQuit()),
               this,SLOT(writeRecentFiles()));

}

int RecentFiles::rowCount(const QModelIndex &parent) const
{
    return stringList.count();
}

QVariant RecentFiles::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= stringList.size())
        return QVariant();

    if (role == FolderRole)
        return stringList.at(index.row()).split("|")[0];
    else if (role == FileRole)
        return stringList.at(index.row()).split("|")[1];
    else
        return QVariant();
}

bool RecentFiles::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent,row,row+count-1);
    while(count--)
        stringList.removeAt(row+count);
    endRemoveRows();
    recentFilesModified = true;
    return true;
}

// Try to read recent files list from the file $HOME/.texteditor/recentfiles.dat
bool RecentFiles::readRecentFiles()
{
    QDir dir;
    QStringList newContent;
    bool retval = true;

    // already tried to load the file names?
    if (recentFilesLoaded)
            return(retval);

    // read file names
    dir.setCurrent(dir.homePath());
    if (dir.exists(CONFIG_FOLDER))
    {
        dir.setCurrent(CONFIG_FOLDER);
        if (dir.exists(RECENT_FILES))
        {
            QFile file(RECENT_FILES);
            bool fileIsReadable = file.open(QIODevice::ReadOnly);
            if (fileIsReadable)
            {
                QDataStream stream( &file );
                stream >> newContent;
                file.close();
                // store the file contents to stringList
                stringList = newContent;
            } else {
                retval = false;
                emit openFailed(RECENT_FILES,file.errorString());
            }
        }
    } else {
        // create the CONFIG_FOLDER if it did not already exist
        dir.mkdir(CONFIG_FOLDER);
    }
    recentFilesLoaded = true;
    return(retval);
}

// Add file specification to the beginning of the list
void RecentFiles::addFile(const QString fileSpec)
{
    // file position in the list
    int index = stringList.indexOf(fileSpec);

    // file is already in the beginning of the list?
    if (index==0)
        return;

    // file is elsewhere in the list?
    if (index>0)
        stringList.removeAt(index);

    // put the new entry to the beginning of the list
    stringList.prepend(fileSpec);

    // limit the list size
    if (stringList.count() > MAX_RECENT_FILES)
        stringList.removeLast();

    recentFilesModified = true;
}

// Remove file definition from the list
void RecentFiles::removeFile(const QString fileSpec)
{
    int nElems = stringList.count();

    // list is empty?
    if (nElems==0)
        return;

    int index = stringList.indexOf(fileSpec);

    // not in the list?
    if (index==-1)
        return;

    // remove the file from the list
    stringList.removeAt(index);
}

// Remove all files from the list
void RecentFiles::clearFiles()
{
    int nElems = stringList.count();
    if(nElems)
        removeRows(0,nElems);
}

// Save the recent files list to file
void RecentFiles::writeRecentFiles()
{
    // Did we modify the recent files list?
    if (!recentFilesModified)
        return;

    QDir dir;
    dir.setCurrent(dir.homePath());
    if (dir.exists(CONFIG_FOLDER))
    {
        dir.setCurrent(CONFIG_FOLDER);

        QFile file(RECENT_FILES);
        bool fileIsWritable = file.open(QIODevice::WriteOnly);

        if ( fileIsWritable ) {
            QDataStream stream( &file );
            stream<<stringList;
            file.close();
        }
    }
}
