#include "accountcontroller.hpp"

#include "common/io/process.hpp"
#include "common/io/processresult.hpp"
#include "logic/nordvpn/envcontroller.hpp"

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
