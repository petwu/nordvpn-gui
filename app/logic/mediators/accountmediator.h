#ifndef ACCOUNTMEDIATOR_H
#define ACCOUNTMEDIATOR_H

#include <QObject>
#include <QString>

#include "logic/nordvpn/accountcontroller.h"

/**
 * @brief The AccountMediator class is responsible for communicating information
 * and actions with the UI that regard the users NordVPN account.
 */
class AccountMediator : public QObject {
    Q_OBJECT

  public slots:
    /**
     * @brief Login to NordVPN's services.
     * @param username The users e-mail or username.
     * @param password The users password.
     * @return True, if the credentials were accepted. False otherwise.
     */
    bool login(QString username, QString password);

    /**
     * @brief Logout the currently logged in user from NordVPN's services.
     * @return True, if the logout was successful.
     */
    bool logout();

  private:
    /**
     * @brief Controller that implements the actual account logic. Used to
     * delegate operations like #login() or #logout() to.
     */
    AccountController _accountController;
};

#endif // ACCOUNTMEDIATOR_H
