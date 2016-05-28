#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeContext>
#include <QDeclarativeItem>
#include <QTranslator>
#include "qmlapplicationviewer.h"
#include "texteditor.h"
#include "recentfiles.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    TextEditor *texteditor;
    RecentFiles *recentfiles;

    QmlApplicationViewer viewer;

    QString locale = QLocale::system().name();
    QTranslator translator;

    // fall back to using English translation, if one specific to the current
    // setting of the device is not available.
    //if (!(translator.load("tr_"+locale, ":/")))
    //    translator.load("tr_en", ":/");

    translator.load("tr_fr",":/");

    app->installTranslator(&translator);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/texteditor/main.qml"));
    viewer.showExpanded();

    // get the QML root object for signal-slot connections
    QObject *qml = viewer.rootObject();

    // Create the recent files list model and put it to the root context
    // Make viewer the parent object.
    recentfiles = new RecentFiles(qml, &viewer);
    viewer.rootContext()->setContextProperty("recentfiles",recentfiles);

    // Create the back-end processor and pass in the root object and the recent files model.
    // Make viewer the parent object.
    texteditor = new TextEditor(qml, recentfiles, &viewer);

    // Catch application exit to save the recent files list
    QObject::connect(QCoreApplication::instance(),SIGNAL(aboutToQuit()),recentfiles,SLOT(closing()));

    // Start the event loop
    return app->exec();
}
