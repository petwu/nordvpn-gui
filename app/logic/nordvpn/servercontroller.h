#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include "basecontroller.h"
#include "data/repositories/serverrepository.h"

class ServerController : public BaseController {
  public:
    json::array_t getAllCountries();
    std::vector<std::string> getCities(std::string country);
    std::vector<std::string> getGroups();
};

#endif // SERVERCONTROLLER_H
