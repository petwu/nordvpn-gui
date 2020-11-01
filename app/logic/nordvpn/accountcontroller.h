#ifndef ACCOUNTCONTROLLER_H
#define ACCOUNTCONTROLLER_H

#include "basecontroller.h"
#include "envcontroller.h"

class AccountController : BaseController {
  public:
    AccountController();
    bool login(std::string username, std::string password);
    bool logout();
};

#endif // ACCOUNTCONTROLLER_H
