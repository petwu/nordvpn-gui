#include "accountcontroller.h"

AccountController::AccountController() {}

bool AccountController::login(std::string username, std::string password) {
    std::string cmd = "nordvpn login --username '" + username +
                      "' --password '" + password + "'";
    if (Process::execute(cmd).success()) {
        EnvController::getInstance().setLoggedIn(true);
        return true;
    }
    return false;
}

bool AccountController::logout() {
    if (Process::execute("nordvpn logout").success()) {
        EnvController::getInstance().setLoggedIn(false);
        return true;
    }
    return false;
}
