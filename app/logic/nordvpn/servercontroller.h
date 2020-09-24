#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include "basecontroller.h"
#include "data/repositories/serverrepository.h"

class ServerController : public BaseController {
  public:
    json::array_t getAllCountries();
    std::vector<std::string> getCities(std::string country);
    std::vector<std::string> getGroups();
    void quickConnect();
    void connectToCountryById(uint32_t id);
    void disconnect();

  private:
    json _allCountries;
};

#endif // SERVERCONTROLLER_H
