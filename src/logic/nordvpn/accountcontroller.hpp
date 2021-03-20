#ifndef ACCOUNTCONTROLLER_HPP
#define ACCOUNTCONTROLLER_HPP

#include <string>

#include "basecontroller.hpp"
#include "iaccountcontroller.hpp"

class AccountController : public virtual IAccountController,
                          public BaseController {
  public:
    auto login(const std::string &username, const std::string &password)
        -> bool override;
    auto logout() -> bool override;
};

#endif // ACCOUNTCONTROLLER_HPP
