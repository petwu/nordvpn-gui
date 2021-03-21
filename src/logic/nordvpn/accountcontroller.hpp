#ifndef ACCOUNTCONTROLLER_HPP
#define ACCOUNTCONTROLLER_HPP

#include <memory>
#include <string>

#include "basecontroller.hpp"
#include "iaccountcontroller.hpp"
#include "logic/nordvpn/ienvcontroller.hpp"

class AccountController : public virtual IAccountController,
                          public BaseController {
  public:
    AccountController(std::shared_ptr<IEnvController> envController);

    auto login(const std::string &username, const std::string &password)
        -> bool override;
    auto logout() -> bool override;

  private:
    const std::shared_ptr<IEnvController> _envController;
};

#endif // ACCOUNTCONTROLLER_HPP
