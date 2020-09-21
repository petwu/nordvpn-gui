#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "logic/mediators/mapmediator.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // create Qt application
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(":/style");
    engine.load("qrc:/main.qml");

    // setup connection between QML/UI and C++/logic through mediator objects
    // that are available as QML context objects
    QQmlContext *ctx = engine.rootContext();
    MapMediator *mapMediator = new MapMediator();
    ctx->setContextProperty("MapMediator", mapMediator);

    return app.exec();
}
