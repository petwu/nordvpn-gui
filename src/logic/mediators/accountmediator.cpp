#include "accountmediator.h"

bool AccountMediator::login(QString username, QString password) {
    return this->_accountController.login(username.toStdString(),
                                          password.toStdString());
}

bool AccountMediator::logout() { //
    return this->_accountController.logout();
}
