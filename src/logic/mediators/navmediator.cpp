#include "navmediator.h"

NavMediator::NavMediator() {
    EnvController &envController = EnvController::getInstance();
    envController.attach(this);
    envController.startBackgroundTask();
}

auto NavMediator::_getMainWindowViewSource() -> QString {
    auto viewSource =
        QString(this->_viewSourceMap[this->_currentMainWindowView].c_str());
    return std::move(viewSource);
}

void NavMediator::_setCurrentMainWindowView(MainWindowView v) {
    if (this->_currentMainWindowView == v) {
        return;
    }
    this->_currentMainWindowView = v;
    this->mainWindowViewSourceChanged(this->_getMainWindowViewSource());
}

void NavMediator::updateEnv(const EnvInfo &envInfo) {
    /*
        priority of false-values:

        1.) if no shell is available, all other values cannot be checked since
            no process can be executed
        2.) if the NordVPN CLI is not installed, thing application is useless
        3.) without an internet connection, no connection to a VPN server can be
            established and the login status cannot be checked
        4.) without being logged in, the NordVPN services cannot be used
    */
    if (!envInfo.shellAvailable) {
        this->_setCurrentMainWindowView(MainWindowView::NoShell);
    } else if (!envInfo.nordvpnInstalled) {
        this->_setCurrentMainWindowView(MainWindowView::NotInstalled);
    } else if (!envInfo.internetConnected) {
        this->_setCurrentMainWindowView(MainWindowView::NoConnection);
    } else if (envInfo.loggedIn == false) {
        this->_setCurrentMainWindowView(MainWindowView::Login);
    } else {
        this->_setCurrentMainWindowView(MainWindowView::Main);
    }
}

void NavMediator::showViewAfterSuccessfulLogin() {
    this->_setCurrentMainWindowView(MainWindowView::Main);
}
