#ifndef ACCOUNTCONTROLLER_H
#define ACCOUNTCONTROLLER_H

#include "basecontroller.h"
#include "envcontroller.h"

/**
 * @brief The AccountController class is repsonsible for all actions concerning
 * the users account, like loggin in or out.
 */
class AccountController : BaseController {
  public:
    /**
     * @brief Try to login with the given credentials.
     * @param username The users e-mail or username.
     * @param password The users password.
     * @return True, in case the login was successful. False, in case the
     * provided credentials were wrong.
     */
    static auto login(const std::string &username, const std::string &password)
        -> bool;

    /**
     * @brief Logout the currently logged in user.
     * @return True, if successful. False otherwise, e.g. if no user is logged
     * in.
     */
    static auto logout() -> bool;
};

#endif // ACCOUNTCONTROLLER_H
