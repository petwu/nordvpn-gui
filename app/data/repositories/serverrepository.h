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

class Location {
  public:
    int32_t id = -1;
    std::string name = "";
    double lat = 0;
    double lng = 0;

    std::string toString(bool oneLine = true);
};

class Server {
  public:
    std::string name = "";
    std::string hostname = "";
    std::string connectName = "";
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
};

#endif // SERVERREPOSITORY_H
