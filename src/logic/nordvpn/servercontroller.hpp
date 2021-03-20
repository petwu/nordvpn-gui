#ifndef SERVERCONTROLLER_HPP
#define SERVERCONTROLLER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "basecontroller.hpp"
#include "common/templates/backgroundtaskable.hpp"
#include "data/enums/group.hpp"
#include "data/models/server.hpp"
#include "data/repositories/serverrepository.hpp"
#include "iservercontroller.hpp"
#include "logic/nordvpn/preferencescontroller.hpp"

class ServerController : public virtual IServerController,
                         public BaseController,
                         public BackgroundTaskable {
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

    auto getAllServers() -> std::vector<Server> override;
    auto getServerByHostname(const std::string &hostname) -> Server override;
    auto getServersByCountry(int32_t countryId) -> std::vector<Server> override;
    auto getServersByCity(int32_t cityId) -> std::vector<Server> override;
    auto getServersByGroup(Group g, int32_t countryId = -1)
        -> std::vector<Server> override;

  private:
    /**
     * @brief Empty, private constructor (part of the sigleton implementation).
     */
    ServerController();

    ServerRepository _serverRepository;
    PreferencesController &_preferencesController =
        PreferencesController::getInstance();

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

#endif // SERVERCONTROLLER_HPP
