#include "accountmediator.hpp"

#include "logic/nordvpn/accountcontroller.hpp"

auto AccountMediator::login(const QString &username, const QString &password)
    -> bool {
    return this->_accountController.login(username.toStdString(),
                                          password.toStdString());
}

auto AccountMediator::logout() -> bool { //
    return this->_accountController.logout();
}
