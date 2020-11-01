#ifndef NORDVPNSETTINGS_H
#define NORDVPNSETTINGS_H

#include <set>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "common/types/nullable.h"
#include "data/enums/protocol.h"
#include "data/enums/technology.h"

/**
 * @brief The WhitelistPortEntry class represents an entry in the ports
 * whitelist. A port number is a 16 bit unsigned integer and thus ranging from 0
 * to 65535. Hence uint16_t would be enough, but choosing uint32_t as a data
 * type allows to represent invalid ports (provided through user input).
 */
class WhitelistPortEntry {
  public:
    /** @brief Start of the port range. */
    uint32_t portFrom = 0;
    /** @brief End of the port range. */
    uint32_t portTo = 0;
    /** @brief Flag that inicates the used protocols. Use the binary or operator
     * to set multple protocols, e.g. Protocol::TCP | Protocol::UDP . */
    Protocol protocolFlag = Protocol::TCP | Protocol::UDP;

    /**
     * @brief operator == for a member-by-member equality check.
     * @param other
     * @return True, if every member variables of the two compared objects
     * match, false otherwise.
     */
    bool operator==(const WhitelistPortEntry &other) const;

    /**
     * @brief operator < is required in order to be used as a std::set<> key.
     * @param other The other object to compare with.
     * @return True, if considered to be smaller. False otherwise.
     */
    bool operator<(const WhitelistPortEntry &other) const;
};

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
    std::vector<std::string> getWhitelistSubnets() const;
    void addSubnetToWhitelist(std::string subnet);
    void updateWhitelistSubnet(int index, std::string subnet);
    void removeSubnetFromWhitelist(int index);
    std::vector<WhitelistPortEntry> getWhitelistPorts() const;
    void addPortsToWhitelist(WhitelistPortEntry p);
    void updatePortsOfWhitelist(int index, WhitelistPortEntry p);
    void removePortsFromWhitelist(int index);

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
    std::vector<std::string> whitelistSubnets;
    std::vector<WhitelistPortEntry> whitelistPorts;
};

#endif // NORDVPNSETTINGS_H
