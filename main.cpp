#include <QtGui/QApplication>
#include <QDeclarativeItem>
#include "qmlapplicationviewer.h"
#include "texteditor.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QmlApplicationViewer viewer;

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile("qml/texteditor/main.qml");
    viewer.showExpanded();

    // Get the QML root object for signal-slot connections.
    QObject *qml = viewer.rootObject();

    // Create the text editor back-end processor and pass in the root object.
    // Make viewer the parent object (takes care of deleting the text editor).
    new TextEditor(qml,&viewer);

    return app->exec();
}
