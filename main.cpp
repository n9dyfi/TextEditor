#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeContext>
#include <QDeclarativeItem>
#include "qmlapplicationviewer.h"
#include "texteditor.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    TextEditor *texteditor;

    QmlApplicationViewer viewer;

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile("qml/texteditor/main.qml");
    viewer.showExpanded();

    // get the QML root object for signal-slot connections
    QObject *qml = viewer.rootObject();

    texteditor = new TextEditor(qml);

    return app->exec();
}
