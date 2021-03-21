#include "accountcontroller.hpp"

#include <utility>

#include <utility>

#include "common/io/process.hpp"
#include "common/io/processresult.hpp"

AccountController::AccountController(
    std::shared_ptr<IEnvController> envController)
    : _envController(std::move(envController)) {}

auto AccountController::login(const std::string &username,
                              const std::string &password) -> bool {
    std::string cmd = "nordvpn login --username '" + username +
                      "' --password '" + password + "'";
    if (Process::execute(cmd).success()) {
        this->_envController->setLoggedIn(true);
        return true;
    }
    return false;
}

auto AccountController::logout() -> bool {
    if (Process::execute("nordvpn logout").success()) {
        this->_envController->setLoggedIn(false);
        return true;
    }
    return false;
}
