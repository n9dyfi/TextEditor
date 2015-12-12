#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeContext>
#include <QDeclarativeItem>
#include <QTranslator>
#include "qmlapplicationviewer.h"
#include "texteditor.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    TextEditor *texteditor;

    QmlApplicationViewer viewer;

    QString locale = QLocale::system().name();
    QTranslator translator;

    // fall back to using English translation, if one specific to the current
    // setting of the device is not available.
    if (!(translator.load("tr_"+locale, ":/")))
        translator.load("tr_en", ":/");

    app->installTranslator(&translator);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/texteditor/main.qml"));
    viewer.showExpanded();

    // get the QML root object for signal-slot connections
    QObject *qml = viewer.rootObject();

    texteditor = new TextEditor(qml);

    return app->exec();
}
