#ifndef SERVERREPOSITORY_H
#define SERVERREPOSITORY_H

#include <vector>

#include "baserepository.h"

enum class Group {
    STANDARD,
    DOUBLE,
    ONION,
    P2P,
    OBFUSCATED,
};

class Connectable {
  public:
    std::string connectName = "";
};

class Location {
  public:
    int32_t id = -1;
    std::string name = "";
    double lat = 0;
    double lng = 0;

    std::string toString(bool oneLine = true);
};

class Country : public Location, public Connectable {
  public:
    double offsetLeft = 0;
    double offsetTop = 0;
    std::vector<Location> cities;

    std::string toString(bool oneLine = true);
};

class Server : public Connectable {
  public:
    std::string name = "";
    std::string hostname = "";
    uint8_t load = 0;
    bool online = false;
    Location country;
    Location city;
    std::vector<Group> groups;

    std::string toString(bool oneLine = true);
};

class ServerRepository : public BaseRepository {
  public:
    static json::array_t getCountriesJSON();
    static std::vector<Server> fetchServers();
    static std::vector<Country> fetchCountries();
};

#endif // SERVERREPOSITORY_H
