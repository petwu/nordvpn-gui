#include "accountcontroller.h"

#include "common/io/process.h"
#include "common/io/processresult.h"
#include "logic/nordvpn/envcontroller.h"

auto AccountController::login(const std::string &username,
                              const std::string &password) -> bool {
    std::string cmd = "nordvpn login --username '" + username +
                      "' --password '" + password + "'";
    if (Process::execute(cmd).success()) {
        EnvController::getInstance().setLoggedIn(true);
        return true;
    }
    return false;
}

auto AccountController::logout() -> bool {
    if (Process::execute("nordvpn logout").success()) {
        EnvController::getInstance().setLoggedIn(false);
        return true;
    }
    return false;
}
