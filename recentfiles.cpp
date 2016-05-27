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
    // to role names "folder" and "file".
    setRoleNames(roles);

    recentFilesLoaded = false;
    recentFilesModified = false;

    connect(this, SIGNAL(openFailed(QString,QString)),
               qml, SLOT(openFailed(QString,QString)));

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

bool RecentFiles::readRecentFiles()
{
    QDir dir;
    QStringList newContent;
    bool retval = true;

    // already tried to load the file names?
    if (recentFilesLoaded)
            return(retval);

    // read file names from the file $HOME/.texteditor/recentfiles.dat
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
                // load the contents to the recentFiles RecentFiles
                store(newContent);
            } else {
                retval = false;
                emit openFailed(RECENT_FILES,file.errorString());
            }
        }
    } else {
        dir.mkdir(CONFIG_FOLDER);
    }
    recentFilesLoaded = true;
    return(retval);
}

// Store list of files (replace current contents)
void RecentFiles::store(const QStringList newContent)
{
    stringList = newContent;
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

// application is closing, save the recent files list
void RecentFiles::closing()
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
