#include "accountmediator.hpp"

#include <utility>

AccountMediator::AccountMediator(
    std::shared_ptr<IAccountController> accountController)
    : _accountController(std::move(accountController)) {}

auto AccountMediator::login(const QString &username, const QString &password)
    -> bool {
    return this->_accountController->login(username.toStdString(),
                                           password.toStdString());
}

auto AccountMediator::logout() -> bool { //
    return this->_accountController->logout();
}
