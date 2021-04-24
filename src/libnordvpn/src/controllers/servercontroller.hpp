#ifndef SERVERCONTROLLER_HPP
#define SERVERCONTROLLER_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "backgroundtaskable.hpp"
#include "basecontroller.hpp"
#include "nordvpn/enums/group.hpp"
#include "nordvpn/ipreferencescontroller.hpp"
#include "nordvpn/iservercontroller.hpp"
#include "nordvpn/models/server.hpp"
#include "repositories/iserverrepository.hpp"

class ServerController : public virtual IServerController,
                         public BaseController,
                         public BackgroundTaskable {
  public:
    ServerController(
        std::shared_ptr<IPreferencesController> preferencesController,
        std::shared_ptr<IServerRepository> serverRepository);

    auto getAllServers() -> std::vector<Server> override;
    auto getServerByHostname(const std::string &hostname) -> Server override;
    auto getServersByCountry(int32_t countryId) -> std::vector<Server> override;
    auto getServersByCity(int32_t cityId) -> std::vector<Server> override;
    auto getServersByGroup(Group g, int32_t countryId = -1)
        -> std::vector<Server> override;

  private:
    const std::shared_ptr<IPreferencesController> _preferencesController;
    const std::shared_ptr<IServerRepository> _serverRepository;

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
