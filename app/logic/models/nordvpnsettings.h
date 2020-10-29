#ifndef NORDVPNSETTINGS_H
#define NORDVPNSETTINGS_H

#include <string>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "common/types/nullable.h"
#include "data/enums/protocol.h"
#include "data/enums/technology.h"

/**
 * @brief The NordVpnSettings class that wraps all settings that are applyable
 * to the NordVPN CLI. The settings are accesible via getters/setters and not
 * directly as attributes since the settings are partially codependent, meaning
 * chaning one property may effect another one. The rules, how the properties
 * depend on each other are implemented inside the setters.
 *
 * The rules are as follows:
 *
 * - If #technology is set to Technology::NordLynx, the #protocol and
 *   #obfuscated settings are unavailable and hence set to null.
 * - The #dns and #cybersec settings are mutually exclusive. Enabling #dns
 *   disabled #cbersec and vice versa.
 */
class NordVpnSettings {
  public:
    bool getAutoconnect() const;
    void setAutoconnect(bool enabled);
    bool getCybersec() const;
    void setCybersec(bool enabled);
    bool getDns() const;
    void setDns(bool enabled);
    std::vector<std::string> getDnsAddresses() const;
    int getMaxNumberOfDnsAddresses() const;
    void setDnsAddresses(std::string addrs[3]);
    void setDnsAddress(int nth, std::string addr);
    bool getKillswitch() const;
    void setKillswitch(bool enabled);
    bool getNotify() const;
    void setNotify(bool enabled);
    Nullable<bool> getObfuscated() const;
    void setObfuscated(bool enabled);
    Nullable<Protocol> getProtocol() const;
    void setProtocol(Protocol p);
    Technology getTechnology() const;
    void setTechnology(Technology t);

    std::string toJSON();
    static NordVpnSettings fromJSON(std::string settingsJson);

  private:
    bool autoconnect = false;
    bool cybersec = false;
    bool dns = false;
    std::vector<std::string> dnsAddresses = {"", "", ""};
    bool killswitch = false;
    bool notify = false;
    Nullable<bool> obfuscated;
    Nullable<Protocol> protocol;
    Technology technology = Technology::Undefined;
};

#endif // NORDVPNSETTINGS_H
