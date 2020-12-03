#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include <atomic>
#include <thread>

#include "basecontroller.h"
#include "data/repositories/preferencesrepository.h"
#include "data/repositories/serverrepository.h"
#include "envcontroller.h"
#include "preferencescontroller.h"

/**
 * @brief ICountriesSubscription is an interface that can be implemented by any
 * class that wants updates of the country list from the #ServerController.
 *
 * @details Call #ServerController::attach() and possibly
 * #ServerController::startBackgroundTask() to start reveciving updates.
 */
class ICountriesSubscription {
  public:
    /**
     * @brief Function that is called by the #ServerController every time there
     * is an update of the list of recently connected countries.
     */
    virtual void updateRecents(const std::vector<Country> &newRecents) = 0;

    /**
     * @brief Function that is called by the #ServerController every time there
     * is an update of the country list.
     */
    virtual void updateCountryList(const std::vector<Country> &countryList) = 0;
};

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
 * (startBackgroundTask()/stopBackgroundTask()) that periodically refreshes the
 * country and server lists. The country list is then propagated via the
 * observer pattern. Obsersers/subscribers have the implement the
 * #ICountriesSubscription interface and call attach(this) in order to get
 * notified. They can also use detach(this) to unsubscribe.
 *
 * In order to prevent multiple instances and possibly multiple background
 * tasks doing the same thing for a different set of subscribers,
 * ServerController is implemented as a singleton.
 */
class ServerController : public BaseController {
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
     * @brief Get the first #Server whose #Server.hostname attribute equals the
     * provided `hostname`.
     */
    auto getServerByHostname(const std::string &hostname) -> Server;

    /**
     * @brief Get a list of all countries.
     * @details The list of countries is cached on the file system to be able to
     * return it faster and to reduce the amount of API requests required. The
     * `updateCache` parameter can be used to force an API request and
     * override the cached list. This should be done rarely (like once per
     * application start). In cache `updateCache` is false and no cached list
     * exists yet, the API request will be performed automatically.
     */
    auto getAllCountries(bool updateCache = false) -> std::vector<Country>;

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
     * @brief Get a list of all countries that have servers that are within a
     * specific server Group.
     */
    auto getCountriesByGroup(Group g) -> std::vector<Country>;

    /**
     * @brief Get a list of servers that are located inside a specific country
     * and are within a server Group. If `countryId` is -1 (or < 0), all
     * countries will be considered.
     */
    auto getServersByGroup(Group g, int32_t countryId = -1)
        -> std::vector<Server>;

    /**
     * @brief Get a list of all countries available.
     */
    auto getRecentCountries() -> std::vector<Country>;

    /**
     * @brief Remove a country from the list of recently connected countries by
     * ID.
     */
    void removeFromRecentsList(uint32_t countryId);

    /**
     * @brief Connect to a VPN server. Let NordVPN decide which country and
     * server is best.
     */
    void quickConnect();

    /**
     * @brief Connect to a VPN server in a specific country. Let NordVPN decide
     * which city and server in this country is best.
     */
    void connectToCountryById(uint32_t id);

    /**
     * @brief Connect to a VPN server in a specific city. Let NordVPN decide
     * which server in this city is best.
     */
    void connectToCityById(uint32_t id);

    /**
     * @brief Connect to a specific VPN server.
     */
    void connectToServerById(uint32_t id);

    /**
     * @brief Connect to a VPN server in a specific server #Group. Let NordVPN
     * decide which country and server is best.
     */
    void connectToSpecialtyGroup(Group g);

    /**
     * @brief Connect to a country within a specific server group. Let NordVPN
     * decide which server in the group and county is best.
     */
    void connectToCountryByIdAndGroup(uint32_t id, Group g);

    /**
     * @brief Abort an connection attempt.
     * @details This kills the process establishing the connection and performs
     * a disconnect. Hence, if already connected, this function has the same
     * effect as #disconnect().
     */
    void cancelConnection() const;

    /**
     * @brief Disconnect from the currently connected VPN server. If not
     * connected, this has no effect.
     */
    static void disconnect();

    /**
     * @brief Attach a subscriber that will be notified by the background task.
     * @param subscriber An object implementing ICountriesSubscription. This
     * object then may call attach(this).
     * @note Don't forget to call detach() in the destructor.
     */
    void attach(ICountriesSubscription *subscriber);

    /**
     * @brief Unsubscriber a subscriber. The background task will continue to
     * run.
     */
    void detach(ICountriesSubscription *subscriber);

    /**
     * @brief Start a task in a background thread that periodically updates the
     * country list and notified all subscribers.
     */
    void startBackgroundTask();

    /**
     * @brief Terminate the background thread.
     */
    void stopBackgroundTask();

  private:
    /**
     * @brief Empty, private constructor (part of the sigleton implementation).
     */
    ServerController();

    /**
     * @brief Controller to get the settings of the NordVPN CLI, because the
     * server list has to be filtered according to the settings.
     */
    PreferencesController &_preferencesController =
        PreferencesController::getInstance();

    /**
     * @brief Internal list of all countries.
     */
    std::vector<Country> _allCountries;

    /**
     * @brief Internal list of all recently connected countries.
     */
    std::vector<Country> _recents;

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
     * @brief Inspect the #ProcessResult of a connection attempt and check
     * whether it was successful. The most common reason it was not successful
     * is, if the user is not logged in. In this case, this information gets
     * passed to the #EnvController that is repsonsible for taking further
     * actions.
     * @param result The #ProcessResult that is returned by #Process of
     * #AsyncProcess in one of the `connectTo*()` methods of this class.
     * @param countryId The ID of the country trying to connect to. This is used
     * to update the list of recently connected countries in case of a
     * successful connection establishment.
     */
    void _checkConnectResult(const ProcessResult &result, int32_t countryId);

    /**
     * @brief The process ID (PID) of the process that is performing a
     * connection establishment. Has a value if < 0 while not connecting.
     */
    pid_t _connectingPid{};

    /**
     * @brief The list of subscribers.
     */
    std::vector<ICountriesSubscription *> _subscribers;

    /**
     * @brief Notify the subscribers about an update of the list of recently
     * connected countries.
     */
    void _notifySubscribersRecents();

    /**
     * @brief Notify the subscribers about an update of the list of countries.
     */
    void _notifySubscribersCountryList();

    /**
     * @brief Bool that is true, while the background task should keep running.
     * @details Atomic bool for thread safety.
     */
    std::atomic<bool> _performBackgroundTask = false;

    /**
     * @brief The background task responsible for periodically updating the
     * server list.
     * @details Should be executed in a detached background thread.
     */
    void _backgroundTaskServerList();

    /**
     * @brief The background task responsible for periodically updating the
     * country list.
     * @details Should be executed in a detached background thread.
     */
    void _backgroundTaskCountryList();
};

#endif // SERVERCONTROLLER_H
