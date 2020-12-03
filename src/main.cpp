#include <memory>

#include <QApplication>
#include <QFontDatabase>
#include <QLocale>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>

#include "app.h"
#include "logic/mediators/accountmediator.h"
#include "logic/mediators/connectionmediator.h"
#include "logic/mediators/navmediator.h"
#include "logic/mediators/preferencesmediator.h"
#include "logic/mediators/recentsmediator.h"
#include "logic/mediators/traymediator.h"
#include "runguard.h"

auto main(int argc, char *argv[]) -> int {
    // set the behavior of application-wide features
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // create Qt application
    QApplication _(argc, argv);
    QApplication::setOrganizationName(ORGANIZATION);
    QApplication::setOrganizationDomain(REPOSITORY);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(VERSION);
    QApplication::setWindowIcon(QIcon(":/icons/icon.ico"));
    QQmlApplicationEngine engine;
    engine.addImportPath(":/Style");

    // run guard that make this app a single instance application
    // (must be used after QApplication was instantiated since the QLocalServer
    // used by RunGuard internally requires a running event loop)
    RunGuard runGuard(APPLICATION_NAME);
    if (!runGuard.tryToRun()) {
        return 0;
    }
    runGuard.setOnSecondaryInstanceBlockedHandler([] {
        // get notified if any further (blocked) instance creations and show the
        // main window (might be minimized to taskbar or tray or hidden behind
        // other windows)
        TrayMediator::getInstance().showMainWindowAction();
    });

    // add translations for current system locale
    QTranslator translator;
    translator.load(QLocale(), "", "", ":/translations");
    QApplication::installTranslator(&translator);

    // setup connection between QML/UI and C++/logic through mediator objects
    // that are available as a QML context objects
    QQmlContext *ctx = engine.rootContext();
    auto accountMediator = std::make_unique<AccountMediator>();
    auto connectionMediator = std::make_unique<ConnectionMediator>();
    auto navMediator = std::make_unique<NavMediator>();
    auto preferencesMediator = std::make_unique<PreferencesMediator>();
    auto recentsMediator = std::make_unique<RecentsMediator>();
    ctx->setContextProperty("AccountMediator", accountMediator.get());
    ctx->setContextProperty("ConnectionMediator", connectionMediator.get());
    ctx->setContextProperty("NavMediator", navMediator.get());
    ctx->setContextProperty("PreferencesMediator", preferencesMediator.get());
    ctx->setContextProperty("RecentsMediator", recentsMediator.get());
    ctx->setContextProperty("TrayMediator", &TrayMediator::getInstance());

    // populate whether to use dark or light colors:
    // hexadecimal color representation uses 8 bit per channel, hence whe values
    // range from 0-255 and the middle value 128 is euqally light and dark:
    // isDark := (r+g+b)/2 < 128    with    r,g,b ∊ { x | 0≤x≤255 ∧ x∊ℤ}
    constexpr int hexColorMiddle = 128;
    auto base = QApplication::palette().base().color();
    bool isDark =
        (base.red() + base.green() + base.blue()) / 3 < hexColorMiddle;
    ctx->setContextProperty("IsDarkTheme", isDark);

    // add custom fonts (e.g. icon fonts)
    int fa5SolidId =
        QFontDatabase::addApplicationFont(":/fonts/fontawesome5-solid");
    int fa5BrandsId =
        QFontDatabase::addApplicationFont(":/fonts/fontawesome5-brands");
    ctx->setContextProperty(
        "Fa5SolidFontFamily",
        QFontDatabase::applicationFontFamilies(fa5SolidId)[0]);
    ctx->setContextProperty(
        "Fa5BrandsFontFamily",
        QFontDatabase::applicationFontFamilies(fa5BrandsId)[0]);
    ctx->setContextProperty(
        "MonospaceFontFamily",
        QFontDatabase::systemFont(QFontDatabase::FixedFont));

    // populate app information to QML
    ctx->setContextProperty("ApplicationName", APPLICATION_NAME);
    ctx->setContextProperty("ApplicationDescription", APPLICATION_DESCRIPTION);
    ctx->setContextProperty("Author", AUTHOR);
    ctx->setContextProperty("RepositoryURL", REPOSITORY);
    ctx->setContextProperty("IssuesURL", ISSUES_BOARD);
    ctx->setContextProperty("Version", VERSION);
    ctx->setContextProperty("License", LICENSE);
    ctx->setContextProperty("LicenseText", LICENSE_TEXT);
    ctx->setContextProperty("DownloadForLinuxURL", NORDVPN_DOWNLOAD_LINUX_URL);
    ctx->setContextProperty("NordAccountURL", NORD_ACCOUNT_URL);

    // load QML entry point
    engine.load("qrc:/ui/windows/MainWindow.qml");

    // enter main loop
    return QApplication::exec();
}
