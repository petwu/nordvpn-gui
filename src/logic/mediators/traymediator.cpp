#include "traymediator.h"

auto TrayMediator::getInstance() -> TrayMediator & {
    static TrayMediator instance;
    return instance;
}

TrayMediator::TrayMediator() {
    // init context menu
    this->_trayContextMenu = std::make_unique<QMenu>();
    this->_trayContextMenu->addAction(      //
        QIcon::fromTheme("window"), "Open", //
        this, &TrayMediator::showMainWindowAction);
    this->_trayContextMenu->addAction(                         //
        QIcon::fromTheme("settings-configure"), "Preferences", //
        this, &TrayMediator::openPreferencesWindowAction);
    this->_trayContextMenu->addAction(                //
        QIcon::fromTheme("application-exit"), "Quit", //
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
}

void TrayMediator::setIconSource(const QString &filename) {
    this->_trayIcon.setIcon(QPixmap(filename));
    if (!this->_trayIcon.isVisible()) {
        this->_trayIcon.show();
    }
}
