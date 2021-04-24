#ifndef IACCOUNTCONTROLLER_HPP
#define IACCOUNTCONTROLLER_HPP

#include <string>

/**
 * @brief The IAccountController interface is repsonsible for all actions
 * concerning the users account, like loggin in or out.
 */
class IAccountController {
  public:
    /**
     * @brief Try to login with the given credentials.
     * @param username The users e-mail or username.
     * @param password The users password.
     * @return True, in case the login was successful. False, in case the
     * provided credentials were wrong.
     */
    virtual auto login(const std::string &username, const std::string &password)
        -> bool = 0;

    /**
     * @brief Logout the currently logged in user.
     * @return True, if successful. False otherwise, e.g. if no user is logged
     * in.
     */
    virtual auto logout() -> bool = 0;
};

#endif // IACCOUNTCONTROLLER_HPP
