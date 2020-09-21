#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include "basecontroller.h"

class ServerController : public BaseController {
  public:
    std::vector<std::string> getAllCountries();
    std::vector<std::string> getCities(std::string country);
    std::vector<std::string> getGroups();
};

#endif // SERVERCONTROLLER_H
