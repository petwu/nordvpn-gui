#include "traymediator.h"

TrayMediator &TrayMediator::getInstance() {
    static TrayMediator instance;
    return instance;
}

TrayMediator::TrayMediator() {
    // init context menu
    this->_trayContextMenu = new QMenu();
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
    this->_trayIcon.setContextMenu(this->_trayContextMenu);
    this->connect(
        &this->_trayIcon, &QSystemTrayIcon::activated,
        [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::ActivationReason::Trigger ||
                reason == QSystemTrayIcon::ActivationReason::MiddleClick)
                this->toggleMainWindowAction();
        });
}

TrayMediator::~TrayMediator() { //
    delete this->_trayContextMenu;
}

void TrayMediator::setIconSource(QString filename) {
    this->_trayIcon.setIcon(QPixmap(filename));
    if (!this->_trayIcon.isVisible())
        this->_trayIcon.show();
}
