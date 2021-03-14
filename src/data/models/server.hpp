#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

#include "connectable.hpp"
#include "data/enums/group.hpp"
#include "data/enums/protocol.hpp"
#include "data/enums/securityprotocol.hpp"
#include "data/enums/technology.hpp"

/**
 * @brief The Server class is a representation of a server. It is modeled after
 * the data one can retrieve from the NordVPN API. It extends Connectable since
 * you can connect to a specific server directly.
 */
class Server : public Connectable {
  public:
    /**
     * @brief Unique server ID as given by the NordVPN API.
     */
    int32_t id = -1;

    /**
     * @brief Human readable server name.
     * Example: Germany #123
     */
    std::string name;

    /**
     * @brief Hostname of the server that may be resolved into an IP address.
     * Example: fr752.nordvpn.com
     */
    std::string hostname;

    /**
     * @brief The load percentage. A value between 0 and 100.
     */
    uint8_t load = 0;

    /**
     * @brief ID of the country the server resides in. Given by the NordVPN API.
     */
    int32_t countryId = -1;

    /**
     * @brief ID of the city the server resides in. Given by the NordVPN API.
     */
    int32_t cityId = -1;

    /**
     * @brief List of server groups the server is part of.
     */
    std::vector<Group> groups;

    /**
     * @brief List of protocols the server supports.
     */
    std::vector<SecurityProtocol> securityProtocols;

    /**
     * @brief Check whether the server supports SecurityProtocols for obfuscated
     * connections.
     * @return
     */
    auto supportsObfuscated() -> bool;

    /**
     * @brief Check whether the server supports a thing thats is called CyberSec
     * by NordVPN. With CyberSec, e.g. vicious Ads and Malware are blocked by
     * the server.
     * @return
     */
    auto supportsCyberSec() -> bool;

    /**
     * @brief Check whether the server supports SecurityProtocols that are
     * applicable to a network Protocol.
     * @param p
     * @return
     */
    auto supportsProtocol(Protocol p) -> bool;

    /**
     * @brief Check whether the server supports SecurityProtocols for the passed
     * Technology.
     * @param t
     * @return
     */
    auto supportsTechnology(Technology t) -> bool;

    /**
     * @brief Convert the object into a JSON string representation.
     * @return
     */
    auto toJSON() -> std::string;

    /**
     * @brief Inverse operation of #toJSON.
     * @param s
     * @return
     */
    static auto fromJSON(const std::string &s) -> Server;
};

#endif // SERVER_HPP
