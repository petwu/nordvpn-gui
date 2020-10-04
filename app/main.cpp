#include <QApplication>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "logic/mediators/mapmediator.h"
#include "logic/mediators/sidepanelmediator.h"
#include "logic/nordvpn/servercontroller.h"

int main(int argc, char *argv[]) {
    // set the behavior of application-wide features
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // create Qt application
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(":/style");

    // setup connection between QML/UI and C++/logic through mediator objects
    // that are available as QML context objects
    QQmlContext *ctx = engine.rootContext();
    std::shared_ptr<ServerController> serverController(new ServerController{});
    auto *mapMediator = new MapMediator(serverController);
    auto *sidePanelMediator = new SidePanelMediator(serverController);
    ctx->setContextProperty("MapMediator", mapMediator);
    ctx->setContextProperty("SidePanelMediator", sidePanelMediator);

    // populate whether to use dark or light colors
    // isDark := (r+g+b)/2 < 128    with    r,g,b ∊ { x | 0≤x≤255 ∧ x∊ℤ}
    auto base = QApplication::palette().base().color();
    bool isDark = (base.red() + base.green() + base.blue()) / 3 < 128;
    ctx->setContextProperty("IsDarkTheme", isDark);

    // load QML entry point
    engine.load("qrc:/main.qml");

    // enter main loop
    return app.exec();
}
