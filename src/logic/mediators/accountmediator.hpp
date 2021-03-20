#ifndef ACCOUNTMEDIATOR_HPP
#define ACCOUNTMEDIATOR_HPP

#include <QByteArrayData>
#include <QObject>
#include <QString>

#include "logic/nordvpn/accountcontroller.hpp"

/**
 * @brief The AccountMediator class is responsible for communicating information
 * and actions with the UI that regard the users NordVPN account.
 */
class AccountMediator : public QObject {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): Qt is out of scope
    Q_OBJECT

  public slots: // NOLINT(readability-redundant-access-specifiers)
    /**
     * @brief Login to NordVPN's services.
     * @param username The users e-mail or username.
     * @param password The users password.
     * @return True, if the credentials were accepted. False otherwise.
     */
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    bool login(const QString &username, const QString &password);

    /**
     * @brief Logout the currently logged in user from NordVPN's services.
     * @return True, if the logout was successful.
     */
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    bool logout();

  private:
    AccountController _accountController;
};

#endif // ACCOUNTMEDIATOR_HPP
