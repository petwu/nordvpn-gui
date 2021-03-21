#include <QApplication>
#include <QCoreApplication>
#include <QFont>
#include <QFontDatabase>
#include <QIcon>
#include <QList>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringList>
#include <QTranslator>
#include <QVariant>
#include <Qt>
#include <memory>

#include "app.hpp"
#include "data/repositories/preferencesrepository.hpp"
#include "data/repositories/serverrepository.hpp"
#include "logic/mediators/accountmediator.hpp"
#include "logic/mediators/connectionmediator.hpp"
#include "logic/mediators/devmediator.hpp"
#include "logic/mediators/mapimageprovider.hpp"
#include "logic/mediators/navmediator.hpp"
#include "logic/mediators/preferencesmediator.hpp"
#include "logic/mediators/recentsmediator.hpp"
#include "logic/mediators/traymediator.hpp"
#include "logic/nordvpn/accountcontroller.hpp"
#include "logic/nordvpn/connectioncontroller.hpp"
#include "logic/nordvpn/countrycontroller.hpp"
#include "logic/nordvpn/envcontroller.hpp"
#include "logic/nordvpn/preferencescontroller.hpp"
#include "logic/nordvpn/recentscontroller.hpp"
#include "logic/nordvpn/servercontroller.hpp"
#include "logic/nordvpn/statuscontroller.hpp"
#include "runguard.hpp"

auto main(int argc, char *argv[]) -> int {
    // set the behavior of application-wide features
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // create Qt application
    QApplication _(argc, argv);
    QApplication::setOrganizationName(ORGANIZATION);
    QApplication::setOrganizationDomain(PROJECT_WEBSITE_URL);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(VERSION);
    QApplication::setWindowIcon(QIcon(":/icons/icon.ico"));
    auto engine = std::make_unique<QQmlApplicationEngine>();
    engine->addImportPath(":/Style");

    // run guard that make this app a single instance application
    // (must be used after QApplication was instantiated since the QLocalServer
    // used by RunGuard internally requires a running event loop)
    RunGuard runGuard(APPLICATION_NAME);
    if (!runGuard.tryToRun()) {
        return 0;
    }

    // add translations for current system locale
    QTranslator translator;
    translator.load(QLocale(), "", "", ":/translations");
    QApplication::installTranslator(&translator);

    // setup connection between QML/UI and C++/logic through mediator objects
    // that are available as a QML context objects
    QQmlContext *ctx = engine->rootContext();

    auto serverRepository = std::make_shared<ServerRepository>();
    auto preferencesRepository = std::make_shared<PreferencesRepository>();

    auto envController = std::make_shared<EnvController>();
    auto accountController = std::make_shared<AccountController>(envController);
    auto preferencesController = std::make_shared<PreferencesController>();
    auto serverController = std::make_shared<ServerController>(
        preferencesController, serverRepository);
    auto statusController =
        std::make_shared<StatusController>(serverController);
    auto countryController =
        std::make_shared<CountryController>(serverController, serverRepository);
    auto recentsController = std::make_shared<RecentsController>(
        countryController, preferencesRepository);
    auto connectionController = std::make_shared<ConnectionController>(
        countryController, envController, recentsController, serverController,
        statusController);

    auto accountMediator = std::make_unique<AccountMediator>(accountController);
    auto connectionMediator = std::make_unique<ConnectionMediator>(
        connectionController, countryController, serverController,
        statusController);
    auto devMediator = std::make_unique<DevMediator>(statusController);
    auto navMediator = std::make_unique<NavMediator>(envController);
    auto preferencesMediator =
        std::make_unique<PreferencesMediator>(preferencesController);
    auto recentsMediator = std::make_unique<RecentsMediator>(recentsController);
    auto trayMediator = std::make_unique<TrayMediator>(
        countryController, recentsController, statusController);

    ctx->setContextProperty("AccountMediator", accountMediator.get());
    ctx->setContextProperty("ConnectionMediator", connectionMediator.get());
    ctx->setContextProperty("DevMediator", devMediator.get());
    ctx->setContextProperty("NavMediator", navMediator.get());
    ctx->setContextProperty("PreferencesMediator", preferencesMediator.get());
    ctx->setContextProperty("RecentsMediator", recentsMediator.get());
    ctx->setContextProperty("TrayMediator", trayMediator.get());

    // get notified if any further (blocked) instance creations and show the
    // main window (might be minimized to taskbar or tray or hidden behind
    // other windows)
    runGuard.setOnSecondaryInstanceBlockedHandler(
        [&trayMediator] { trayMediator->showMainWindowAction(); });

    // additional resource providers
    auto mapImageProvider = std::make_unique<MapImageProvider>();
    // call release() since QQmlEngine takes over ownership and otherwise a
    // double-delete would occur
    engine->addImageProvider("map", mapImageProvider.release());

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
#if IS_DEBUG
    ctx->setContextProperty("IsDebug", true);
#endif
    ctx->setContextProperty("ApplicationName", APPLICATION_NAME);
    ctx->setContextProperty("ApplicationDescription", APPLICATION_DESCRIPTION);
    ctx->setContextProperty("Author", AUTHOR);
    ctx->setContextProperty("RepositoryURL", REPOSITORY_URL);
    ctx->setContextProperty("ProjectWebsiteURL", PROJECT_WEBSITE_URL);
    ctx->setContextProperty("DocsURL", DOCS_URL);
    ctx->setContextProperty("DonateURL", DONATE_URL);
    ctx->setContextProperty("IssuesURL", ISSUES_URL);
    ctx->setContextProperty("Version", VERSION);
    ctx->setContextProperty("License", LICENSE);
    ctx->setContextProperty("LicenseText", LICENSE_TEXT);
    ctx->setContextProperty("DownloadForLinuxURL", NORDVPN_DOWNLOAD_LINUX_URL);
    ctx->setContextProperty("NordAccountURL", NORD_ACCOUNT_URL);

    // load QML entry point
    engine->load("qrc:/ui/windows/MainWindow.qml");

    // enter main loop
    auto rc = QApplication::exec();

    // Delete the QQmlEngine object before all *Mediator objects get deleted due
    // to the return. Without this, a bunch of errors like e.g.
    //   qrc:/ui/map/Marker.qml:25: TypeError: Cannot read property
    //   'connectedCountryId' of null
    // will be raised. See also the following bug report on this issue:
    // https://bugreports.qt.io/browse/QTBUG-81247?focusedCommentId=512347#comment-512347
    // This does not have a big impact when starting via a .desktop shortcut
    // since the application should be quit anyway, but it is ugly when started
    // via the command line.
    engine.reset();
    return rc;
}
