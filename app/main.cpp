#include <QApplication>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "logic/mediators/mediator.h"

int main(int argc, char *argv[]) {
    // set the behavior of application-wide features
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // create Qt application
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(":/style");

    // setup connection between QML/UI and C++/logic through a mediator object
    // that is available as a QML context object
    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("Mediator", new Mediator());

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
