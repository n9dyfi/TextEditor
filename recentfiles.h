#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QStringList>
#include <QAbstractListModel>
#include <QDesktopServices>
#include <QDebug>

class RecentFiles : public QAbstractListModel
{
    Q_OBJECT

public:
    RecentFiles(QObject *qml, QObject *parent = 0);

    enum RecentRoles {
        FolderRole = Qt::UserRole + 1,
        FileRole
    };

    // When subclassing QAbstractListModel, we must provide implementations of the
    // rowCount() and data() functions.
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    // Methods for internal data processing
    bool readRecentFiles();
    void store(const QStringList newContent);
    void addFile(const QString fileSpec);
    void removeFile(const QString fileSpec);

private:
    static const char *CONFIG_FOLDER;
    static const char *RECENT_FILES;
    static const int MAX_RECENT_FILES = 8;

    QStringList stringList;

    // Load the recent files list when 1) opening the recent files page
    // 2) Save As succeeded 3) Open succeeded.
    bool recentFilesLoaded;
    bool recentFilesModified;

signals:
    void openFailed(QString fileName, QString errorString);

public slots:
    void closing();
};

#endif // RECENTFILES_H
