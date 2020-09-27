#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include "basecontroller.h"
#include "data/repositories/serverrepository.h"

class ServerController : public BaseController {
  public:
    std::vector<Country> getAllCountries();
    std::vector<std::string> getCities(std::string country);
    std::vector<std::string> getGroups();
    void quickConnect();
    void connectToCountryById(uint32_t id);
    void disconnect();

  private:
    std::vector<Country> _allCountries;
};

#endif // SERVERCONTROLLER_H
