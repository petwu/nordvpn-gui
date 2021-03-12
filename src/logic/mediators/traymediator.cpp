#include "traymediator.h"

#include <QApplication>
#include <QArgument>
#include <QCoreApplication>
#include <QIcon>
#include <QMetaObject>
#include <QPixmap>
#include <QTimer>
#include <Qt>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "logic/enums/connectionstatus.h"
#include "logic/nordvpn/countrycontroller.h"
#include "logic/nordvpn/recentscontroller.h"
#include "logic/nordvpn/statuscontroller.h"

auto TrayMediator::getInstance() -> TrayMediator & {
    static TrayMediator instance;
    return instance;
}

TrayMediator::TrayMediator() {
    // init context menu
    this->_trayContextMenu = std::make_unique<QMenu>();
    this->_statusAction = this->_trayContextMenu->addAction("-");
    this->_statusAction->setEnabled(false);
    this->_quickConnectAction = this->_trayContextMenu->addAction( //
        QIcon::fromTheme("network-wired"), tr("Quick Connect"),    //
        this, &TrayMediator::quickConnectAction);
    this->_cancelConnectingAction = this->_trayContextMenu->addAction( //
        QIcon::fromTheme("process-stop"), tr("Cancel"),                //
        this, &TrayMediator::cancelConnectingAction);
    this->_disconnectAction = this->_trayContextMenu->addAction( //
        QIcon::fromTheme("network-offline"), tr("Disconnect"),   //
        this, &TrayMediator::disconnectAction);

    this->_trayContextMenu->addSeparator();

    this->_countriesSubmenu = std::make_unique<QMenu>(tr("Countries"));
    this->_countriesSubmenu->setIcon(QIcon::fromTheme("flag"));
    this->_trayContextMenu->addMenu(this->_countriesSubmenu.get());

    this->_recentsSubmenu = std::make_unique<QMenu>(tr("Recents"));
    this->_recentsSubmenu->setIcon(QIcon::fromTheme("edit-undo"));
    this->_trayContextMenu->addMenu(this->_recentsSubmenu.get());

    this->_trayContextMenu->addSeparator();

    this->_trayContextMenu->addAction(          //
        QIcon::fromTheme("window"), tr("Open"), //
        this, &TrayMediator::showMainWindowAction);
    this->_trayContextMenu->addAction(                             //
        QIcon::fromTheme("settings-configure"), tr("Preferences"), //
        this, &TrayMediator::openPreferencesWindowAction);
    this->_trayContextMenu->addAction(                    //
        QIcon::fromTheme("application-exit"), tr("Quit"), //
        this, &TrayMediator::quitApplicationAction);

    // init tray icon
    this->_trayIcon.setContextMenu(this->_trayContextMenu.get());
    TrayMediator::connect(
        &this->_trayIcon, &QSystemTrayIcon::activated,
        [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::ActivationReason::Trigger ||
                reason == QSystemTrayIcon::ActivationReason::MiddleClick) {
                this->toggleMainWindowAction();
            }
        });

    // subsribe to controller to get updates about the connection status and
    // country/recents lists
    StatusController::getInstance().attach(this, true);
    CountryController::getInstance().attach(this, true);
    RecentsController::getInstance().attach(this, true);
}

TrayMediator::~TrayMediator() {
    StatusController::getInstance().detach(this);
    CountryController::getInstance().detach(this);
}

void TrayMediator::setIconSource(const QString &filename) {
    this->_trayIcon.setIcon(QPixmap(filename));
    if (!this->_trayIcon.isVisible()) {
        this->_trayIcon.show();
    }
}

// dispatch some function to the main UI thread
void dispatch(const std::function<void()> &callback) {
    // use a single shot timer to queue the callback in the main UI thread
    auto *timer = new QTimer(); // NOLINT(cppcoreguidelines-owning-memory): Qt
                                // has it's own memory management, that does not
                                // play well with unique_ptr<> and similar
    timer->moveToThread(QApplication::instance()->thread());
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, [=]() {
        // this gets executed on the main thread
        callback();
    });
    // start the timer that times out immediately and then executes the callback
    QMetaObject::invokeMethod(timer, "start", Qt::QueuedConnection,
                              Q_ARG(int, 0));
}

void TrayMediator::updateConnectionInfo(const ConnectionInfo &newInfo) {
    dispatch([&newInfo, this] {
        // set status text
        this->_statusAction->setText(tr("Status:").append(" ").append(
            QString(connectionStatusToString(newInfo.status).c_str())));
        // toggle action visibility
        this->_quickConnectAction->setVisible(
            (newInfo.status == ConnectionStatus::Disconnected));
        this->_cancelConnectingAction->setVisible(
            (newInfo.status == ConnectionStatus::Connecting));
        this->_disconnectAction->setVisible(
            (newInfo.status == ConnectionStatus::Connected));
    });
}

void TrayMediator::updateRecents(const std::vector<Country> &newRecents) {
    // update the context menu: add a QAction for every recent connection, to
    // quickly reconnect
    dispatch([newRecents, this] {
        this->_recentsSubmenu->clear();
        for (const auto &r : newRecents) {
            auto countryId = r.id;
            this->_recentsSubmenu->addAction(
                QIcon(QString(":/flag/").append(r.countryCode.c_str())),
                QString(r.name.c_str()),
                [countryId, this] { this->connectToCountryById(countryId); });
        }
        this->_recentsSubmenu->menuAction()->setVisible(!newRecents.empty());
        // TODO: fix: tray submenu appears off screen the first time opened
        // without an explicit geometry update, the submenu might be display
        // wrong (partially out of screen) the first time it is opened after the
        // update
        this->_recentsSubmenu->show();
        this->_recentsSubmenu->hide();
    });
}

void TrayMediator::updateCountryList(const std::vector<Country> &countryList) {
    // sum up the countries IDs to check if the list of countries has changed
    // since the last time --> if not: skip
    static uint64_t lastIdSum;
    int64_t idSum = 0;
    for (const auto &c : countryList) {
        idSum += c.id;
    }
    if (idSum == lastIdSum) {
        return;
    }
    lastIdSum = idSum;
    // update the context menu: add a QAction for every country, to quickly
    // connect to that country
    dispatch([countryList, this] {
        this->_countriesSubmenu->clear();
        for (const auto &c : countryList) {
            auto countryId = c.id;
            this->_countriesSubmenu->addAction(
                QIcon(QString(":/flag/").append(c.countryCode.c_str())),
                QString(c.name.c_str()),
                [countryId, this] { this->connectToCountryById(countryId); });
        }
        // TODO: fix: tray submenu appears off screen the first time opened
        // without an explicit geometry update, the submenu might be display
        // wrong (partially out of screen) the first time it is opened after the
        // update
        this->_countriesSubmenu->show();
        this->_countriesSubmenu->hide();
    });
}
