#include <QApplication>
#include <QDebug>
#include <QFontDatabase>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "app.h"
#include "logic/mediators/accountmediator.h"
#include "logic/mediators/mediator.h"
#include "logic/mediators/navmediator.h"
#include "logic/mediators/preferencesmediator.h"
#include "runguard.h"

int main(int argc, char *argv[]) {
    RunGuard guard(APPLICATION_NAME);
    if (!guard.tryToRun())
        return 0;

    // set the behavior of application-wide features
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // create Qt application
    QApplication app(argc, argv);
    app.setOrganizationName(ORGANIZATION);
    app.setOrganizationDomain(REPOSITORY);
    app.setApplicationName(APPLICATION_NAME);
    app.setApplicationVersion(VERSION);
    QQmlApplicationEngine engine;
    engine.addImportPath(":/Style");

    // setup connection between QML/UI and C++/logic through a mediator object
    // that is available as a QML context object
    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("Mediator", new Mediator());
    ctx->setContextProperty("NavMediator", new NavMediator());
    ctx->setContextProperty("AccountMediator", new AccountMediator());
    ctx->setContextProperty("PreferencesMediator", new PreferencesMediator());

    // populate whether to use dark or light colors
    // isDark := (r+g+b)/2 < 128    with    r,g,b ∊ { x | 0≤x≤255 ∧ x∊ℤ}
    auto base = QApplication::palette().base().color();
    bool isDark = (base.red() + base.green() + base.blue()) / 3 < 128;
    ctx->setContextProperty("IsDarkTheme", isDark);

    // add custom fonts (e.g. icon fonts)
    QFontDatabase fontDb;
    int fa5SolidId = fontDb.addApplicationFont(":/fonts/fontawesome5-solid");
    int fa5BrandsId = fontDb.addApplicationFont(":/fonts/fontawesome5-brands");
    ctx->setContextProperty("Fa5SolidFontFamily",
                            fontDb.applicationFontFamilies(fa5SolidId)[0]);
    ctx->setContextProperty("Fa5BrandsFontFamily",
                            fontDb.applicationFontFamilies(fa5BrandsId)[0]);

    // populate app information to QML
    ctx->setContextProperty("ApplicationName", APPLICATION_NAME);
    ctx->setContextProperty("Author", AUTHOR);
    ctx->setContextProperty("RepositoryURL", REPOSITORY);
    ctx->setContextProperty("IssuesURL", ISSUES_BOARD);
    ctx->setContextProperty("Version", VERSION);
    ctx->setContextProperty("DownloadForLinuxURL", NORDVPN_DOWNLOAD_LINUX_URL);
    ctx->setContextProperty("NordAccountURL", NORD_ACCOUNT_URL);

    // load QML entry point
    engine.load("qrc:/ui/windows/MainWindow.qml");

    // enter main loop
    return app.exec();
}
