#include "accountmediator.h"

#include "logic/nordvpn/accountcontroller.h"

auto AccountMediator::login(const QString &username, const QString &password)
    -> bool {
    return AccountController::login(username.toStdString(),
                                    password.toStdString());
}

auto AccountMediator::logout() -> bool { //
    return AccountController::logout();
}
