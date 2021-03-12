#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include <cstdint>
#include <string>
#include <vector>

#include "basecontroller.h"
#include "common/templates/backgroundtaskable.h"
#include "data/enums/group.h"
#include "data/models/server.h"

/**
 * @brief The ServerController class is repsonsible for getting informations
 * about servers and allowing to connect to one of them.
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
 * This class provides the functionality to perform every of the five connection
 * requests above as well as the therefore required informations like country or
 * server lists.
 *
 * The class also provides a mechanism for running a background task (thread)
 * (startBackgroundTasks()/stopBackgroundTasks()) that periodically refreshes
 * the country and server lists. The country list is then propagated via the
 * observer pattern. Obsersers/subscribers have the implement the
 * #ICountriesSubscription interface and call attach(this) in order to get
 * notified. They can also use detach(this) to unsubscribe.
 *
 * In order to prevent multiple instances and possibly multiple background
 * tasks doing the same thing for a different set of subscribers,
 * ServerController is implemented as a singleton.
 */
class ServerController : public BaseController, public BackgroundTaskable {
    // Singleton:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
  public:
    ServerController(const ServerController &) = delete;
    void operator=(const ServerController &) = delete;
    ServerController(ServerController &&) = delete;
    auto operator=(ServerController &&) -> ServerController & = delete;
    ~ServerController() = default;

    /**
     * @brief Get the singleton instance of ServerController.
     * @details The instance will be constructed if it does not exist already.
     */
    static auto getInstance() -> ServerController &;

    /**
     * @brief Get a list of all servers.
     */
    auto getAllServers() -> std::vector<Server>;

    /**
     * @brief Get the first #Server whose #Server.hostname attribute equals the
     * provided `hostname`.
     */
    auto getServerByHostname(const std::string &hostname) -> Server;

    /**
     * @brief Get a list of all servers in a country.
     * @param countryId The ID of the country. If the ID does not exists, no
     * server will be returned.
     */
    auto getServersByCountry(int32_t countryId) -> std::vector<Server>;

    /**
     * @brief Get a list of all servers in a city/region.
     * @param cityId The ID of the city. If the ID does not exists, no server
     * will be returned.
     */
    auto getServersByCity(int32_t cityId) -> std::vector<Server>;

    /**
     * @brief Get a list of servers that are located inside a specific country
     * and are within a server Group. If `countryId` is -1 (or < 0), all
     * countries will be considered.
     */
    auto getServersByGroup(Group g, int32_t countryId = -1)
        -> std::vector<Server>;

  private:
    /**
     * @brief Empty, private constructor (part of the sigleton implementation).
     */
    ServerController();

    /**
     * @brief Internal list of all servers.
     */
    std::vector<Server> _allServers;

    /**
     * @brief Filter the list of servers according to the NordVPN CLI settings
     * and the provided country/city ID.
     * @details The following settings influce the server list, because
     * depending on their value, some servers are either available or not:
     * - Obfuscated
     * - Cybersec
     * - Network #Protocol
     * - VPN #Technology
     * @see #Server::supportsObfuscated(), #Server::supportsCyberSec(),
     * #Server::supportsProtocol(), #Server::supportsTechnology()
     * @param countryId The ID of the country, a server has to be located in.
     * Use a value < 0 to include all countries.
     * @param cityId The ID of the city, a server has to be located in.
     * Use a value < 0 to include all cities.
     */
    auto _filterServerList(int32_t countryId, int32_t cityId)
        -> std::vector<Server>;

    /**
     * @brief The background task responsible for periodically updating the
     * server list.
     */
    void _backgroundTask();
};

#endif // SERVERCONTROLLER_H
