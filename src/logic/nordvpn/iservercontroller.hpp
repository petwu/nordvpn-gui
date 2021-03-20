#ifndef ISERVERCONTROLLER_HPP
#define ISERVERCONTROLLER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "common/templates/ibackgroundtaskable.hpp"
#include "data/enums/group.hpp"
#include "data/models/server.hpp"

/**
 * @brief The IServerController interface is repsonsible for getting
 * informations about servers and allowing to connect to one of them.
 *
 * @details With NordVPN there are different possibilities on how to specify the
 * server one wants to connect to. You can state ...
 * 1. "I want to connect"
 * 2. "I want to connect to country XY"
 * 3. "I want to connect to city XY"
 * 4. "I want to connect to server group XY"
 * 5. "I want to connect to server XY"
 * .
 * In every case except for (5) NordVPN will select to best server
 * automatically. In case (1) theoretically every server is possible, but
 * NordVPN will most likely pick one near the users grographical location to
 * reduce latency. (2) to (4) simply provide means to restrict the list of
 * possible servers to a more or less specific region or to a group of servers
 * with special features (like Double VPN, Onion over VPN, etc.).
 *
 * This interface provides the functionality to perform every of the five
 * connection requests above.
 * The interface also provide
 */
class IServerController : public virtual IBackgroundTaskable {
  public:
    /**
     * @brief Get a list of all servers.
     */
    virtual auto getAllServers() -> std::vector<Server> = 0;

    /**
     * @brief Get the first #Server whose #Server.hostname attribute equals the
     * provided `hostname`.
     */
    virtual auto getServerByHostname(const std::string &hostname) -> Server = 0;

    /**
     * @brief Get a list of all servers in a country.
     * @param countryId The ID of the country. If the ID does not exists, no
     * server will be returned.
     */
    virtual auto getServersByCountry(int32_t countryId)
        -> std::vector<Server> = 0;

    /**
     * @brief Get a list of all servers in a city/region.
     * @param cityId The ID of the city. If the ID does not exists, no server
     * will be returned.
     */
    virtual auto getServersByCity(int32_t cityId) -> std::vector<Server> = 0;

    /**
     * @brief Get a list of servers that are located inside a specific country
     * and are within a server Group. If `countryId` is -1 (or < 0), all
     * countries will be considered.
     */
    virtual auto getServersByGroup(Group g, int32_t countryId = -1)
        -> std::vector<Server> = 0;
};

#endif // ISERVERCONTROLLER_HPP
