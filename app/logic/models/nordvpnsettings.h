#ifndef NORDVPNSETTINGS_H
#define NORDVPNSETTINGS_H

#include <string>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "data/enums/protocol.h"
#include "data/enums/technology.h"

class NordVpnSettings {
  public:
    bool autoconnect = false;
    bool cybersec = false;
    bool dns = false;
    std::string dnsAddresses[3] = {"", "", ""};
    bool killswitch = false;
    bool notify = false;
    bool obfuscated = false;
    Protocol protocol = Protocol::Undefined;
    Technology technology = Technology::Undefined;

    std::string toJSON();
    static NordVpnSettings fromJSON(std::string settingsJson);
};

#endif // NORDVPNSETTINGS_H
