#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "connectable.h"
#include "data/enums/group.h"
#include "data/enums/protocol.h"
#include "data/enums/securityprotocol.h"
#include "data/enums/technology.h"
#include "location.h"

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

#endif // SERVER_H
