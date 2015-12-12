#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QDeclarativeItem>
#include <QFile>

#define SAVE 0
#define SAVE_AS 1

class TextEditor : public QObject
{
    Q_OBJECT

public:
    explicit TextEditor(QObject *qml, QObject *parent = 0);

private:
    static const QString DEFAULT_FILE;

    QString currentFolder;
    QString currentFile;
    QString currentContent;

private slots:
    void saveCurrentContent(int);

public slots:
    void menuOpenClicked(QString);
    void menuSaveClicked(QString);
    void menuSaveAsClicked();
    void saveAsRequested(QString,QString);
    void currentFolderChanged(QString);
    void fileOpenRequested(QString);
    void saveAsConfirmed(QString);
    void openConfirmed();
    void newConfirmed();
    void appCloseRequested(QString);
    void saveBeforeClosed(QString);
    void fileNewRequested(QString);

signals:
    void browseRequested(QString currentFolder, bool saveRequested);
    void openCompleted(QString content, QString currentFolder, QString currentFile);
    void openFailed(QString fileName, QString errorString);
    void saveCompleted(QString currentFolder, QString currentFile);
    void saveFailed(QString fileName, QString errorString);
    void saveAsCompleted(QString currentFolder, QString currentFile);
    void saveAsToBeConfirmed(QString fileName);
    void openToBeConfirmed(QString fileName);
    void newToBeConfirmed(QString fileName);
    void appCloseToBeConfirmed(QString fileName);
    void appToBeClosed();
    void editorCleared(QString currentFolder, QString currentFile);
};

#endif // TEXTEDITOR_H
