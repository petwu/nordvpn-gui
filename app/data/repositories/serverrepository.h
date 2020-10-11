#ifndef SERVERREPOSITORY_H
#define SERVERREPOSITORY_H

#include <vector>

#include "baserepository.h"

class Connectable {
  public:
    std::string connectName = "";
};

class Location {
  public:
    int32_t id = -1;
    std::string name = "";
    float lat = 0;
    float lng = 0;
};

class Country : public Location, public Connectable {
  public:
    std::string countryCode;
    float offsetLeft = 0;
    float offsetTop = 0;
    std::vector<Location> cities;
};

class Server : public Connectable {
  public:
    int32_t id = -1;
    std::string name = "";
    std::string hostname = "";
    uint8_t load = 0;
    int32_t countryId = -1;
    int32_t cityId = -1;
    std::vector<Group> groups;
    std::vector<SecurityProtocol> securityProtocols;

    bool supportsObfuscated();
    bool supportsCyberSec();
    bool supportsProtocol(Protocol p);
    bool supportsTechnology(Technology t);

    std::string toJSON();
    static Server fromJSON(const std::string &s);
};

class ServerRepository : public BaseRepository {
  public:
    static std::vector<Server> fetchServers();
    static std::vector<Server> fetchServersFromCache();
    static std::vector<Country> fetchCountries();

  private:
    static json::array_t getCountriesJSON();
    static void saveServerListToFile(std::vector<Server> &serverList);
};

#endif // SERVERREPOSITORY_H
