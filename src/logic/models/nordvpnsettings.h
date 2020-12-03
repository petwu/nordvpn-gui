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
#include "logic/models/whitelistportentry.h"

/**
 * @brief The NordVpnSettings class that wraps all settings that are applyable
 * to the NordVPN CLI. The settings are accesible via getters/setters and not
 * directly as attributes since the settings are partially codependent, meaning
 * chaning one property may effect another one. The rules, how the properties
 * depend on each other are implemented inside the setters.
 *
 * The rules are as follows:
 *
 * - If `technology` is set to Technology::NordLynx, the `protocol` and
 *   `obfuscated` settings are unavailable and hence set to null.
 * - The `dns` and `cybersec` settings are mutually exclusive. Enabling `dns`
 *   disabled `cbersec` and vice versa.
 */
class NordVpnSettings {
  public:
    /**
     * @brief Get whether autoconnect is enabled or not.
     */
    auto getAutoconnect() const -> bool;

    /**
     * @brief Set whether autoconnect is enabled or not.
     */
    void setAutoconnect(bool enabled);

    /**
     * @brief Get whether cybersec is enabled or not.
     */
    auto getCybersec() const -> bool;

    /**
     * @brief Set whether cybersec is enabled or not.
     */
    void setCybersec(bool enabled);

    /**
     * @brief Get whether custom DNS is enabled or not.
     */
    auto getDns() const -> bool;

    /**
     * @brief Set whether custom DNS is enabled or not.
     */
    void setDns(bool enabled);

    /**
     * @brief Get the list of custom DNS addresses.
     * @details The list has a fixed size (see getMaxNumberOfDnsAddresses())
     * and might contain empty string.
     */
    auto getDnsAddresses() const -> std::vector<std::string>;

    /**
     * @brief Return the max. number of custom DNS addresses that can be set.
     * The limit is specified by the NordVPN CLI.
     */
    auto getMaxNumberOfDnsAddresses() const -> int;

    /**
     * @brief Set the nth custom DNS address.
     */
    void setDnsAddress(int nth, const std::string &addr);

    /**
     * @brief Get whether the killswitch is enabled or not.
     */
    auto getKillswitch() const -> bool;

    /**
     * @brief Set whether the killswitch is enabled or not.
     */
    void setKillswitch(bool enabled);

    /**
     * @brief Get whether notifications are enabled or not.
     */
    auto getNotify() const -> bool;

    /**
     * @brief Set whether notifications are enabled or not.
     */
    void setNotify(bool enabled);

    /**
     * @brief Get whether obfuscated connections are enabled or not.
     * @return A Nullable bool and not a simple bool, since depending on the
     * #Technology, the obfuscated setting might be unavailable. In this case a
     * null value is returned. This can be checked with Nullable::isNull().
     */
    auto getObfuscated() const -> Nullable<bool>;

    /**
     * @brief Set whether obfuscated connections are enabled or not.
     * @details Depending on the #Technology, the obfuscated setting might not
     * be available. In this case, calling this function has no effect.
     */
    void setObfuscated(bool enabled);

    /**
     * @brief Get the network protocol setting.
     * @return A Nullable bool and not a simple bool, since depending on the
     * #Technology, the #Protocol setting might be unavailable. In this case a
     * null value is returned. This can be checked with Nullable::isNull().
     */
    auto getProtocol() const -> Nullable<Protocol>;

    /**
     * @brief Set the network #Protocol used for connections.
     * @details Depending on the #Technology, the #Protocol setting might not be
     * available. In this case, calling this function has no effect.
     */
    void setProtocol(Protocol p);

    /**
     * @brief Get the #Technology setting.
     */
    auto getTechnology() const -> Technology;

    /**
     * @brief Set the #Technology used for connections.
     */
    void setTechnology(Technology t);

    /**
     * @brief Get a list of whitelisted subnets.
     */
    auto getWhitelistSubnets() const -> std::vector<std::string>;

    /**
     * @brief Add a subnet to the whitelist.
     */
    void addSubnetToWhitelist(const std::string &subnet);

    /**
     * @brief Update the value of a whitelist entry by index.
     */
    void updateWhitelistSubnet(int index, std::string subnet);

    /**
     * @brief Remove an entry from the whitelisted subnets by index.
     */
    void removeSubnetFromWhitelist(int index);

    /**
     * @brief Get a list of whitelisted ports/port ranges and the network
     * #Protocol they apply to.
     */
    auto getWhitelistPorts() const -> std::vector<WhitelistPortEntry>;

    /**
     * @brief Add a port/port range to the whitelist.
     */
    void addPortsToWhitelist(WhitelistPortEntry p);

    /**
     * @brief Update the value of a whitelist entry by index.
     */
    void updatePortsOfWhitelist(int index, WhitelistPortEntry p);

    /**
     * @brief Remove an entry from the whitelisted ports/port ranges by index.
     */
    void removePortsFromWhitelist(int index);

  private:
    /**
     * @brief Whether autoconnect is enabled or not.
     */
    bool autoconnect = false;

    /**
     * @brief Whether autoconnect is enabled or not.
     */
    bool cybersec = false;

    /**
     * @brief Whether autoconnect is enabled or not.
     */
    bool dns = false;

    /**
     * @brief Whether cybersec is enabled or not.
     */
    std::vector<std::string> dnsAddresses = {"", "", ""};

    /**
     * @brief Whether the killswitch is enabled or not.
     */
    bool killswitch = false;

    /**
     * @brief Whether desktop notifications are enabled or not.
     */
    bool notify = false;

    /**
     * @brief Whether obfuscated connections are enabled or not.
     * @details Depending on the #Technology, this settings is unavailable and
     * will be null.
     */
    Nullable<bool> obfuscated;

    /**
     * @brief The network #Protocol used for connections.
     * @details Depending on the #Technology, this settings is unavailable and
     * will be null.
     */
    Nullable<Protocol> protocol;

    /**
     * @brief The VPN #Technology used for connections.
     */
    Technology technology = Technology::Undefined;

    /**
     * @brief The list of whitelisted subnets.
     */
    std::vector<std::string> whitelistSubnets;

    /**
     * @brief The list of whitelisted ports/port ranges.
     */
    std::vector<WhitelistPortEntry> whitelistPorts;
};

#endif // NORDVPNSETTINGS_H
