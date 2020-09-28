#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "logic/mediators/mapmediator.h"
#include "logic/mediators/sidepanelmediator.h"

int main(int argc, char *argv[]) {
    // set the behavior of application-wide features
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // create Qt application
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(":/style");

    // setup connection between QML/UI and C++/logic through mediator objects
    // that are available as QML context objects
    QQmlContext *ctx = engine.rootContext();
    auto *mapMediator = new MapMediator();
    auto *sidePanelMediator = new SidePanelMediator();
    ctx->setContextProperty("MapMediator", mapMediator);
    ctx->setContextProperty("SidePanelMediator", sidePanelMediator);

    // load QML entry point
    engine.load("qrc:/main.qml");

    // enter main loop
    return app.exec();
}
