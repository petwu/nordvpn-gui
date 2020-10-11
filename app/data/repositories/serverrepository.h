#ifndef SERVERREPOSITORY_H
#define SERVERREPOSITORY_H

#include <vector>

#include "baserepository.h"
#include "data/models/country.h"
#include "data/models/server.h"

class ServerRepository : public BaseRepository {
  public:
    static std::vector<Server> fetchServers();
    static std::vector<Server> fetchServersFromCache();
    static std::vector<Country> fetchCountries();
    static std::vector<Country> fetchCountriesFromCache();

  private:
    static json::array_t getCountriesJSON();
    static void saveServerListToFile(std::vector<Server> &serverList);
    static void saveCountryListToFile(std::vector<Country> &countryList);
};

#endif // SERVERREPOSITORY_H
