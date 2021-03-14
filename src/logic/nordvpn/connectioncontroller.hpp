#ifndef CONNECTIONCONTROLLER_HPP
#define CONNECTIONCONTROLLER_HPP

#include <cstdint>
#include <sys/types.h>

#include "common/io/processresult.hpp"
#include "data/enums/group.hpp"
#include "logic/models/connectioninfo.hpp"
#include "logic/subscriptions/iconnectioninfosubscription.hpp"

class ConnectionController : public IConnectionInfoSubscription {
    // Singleton:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
  public:
    ConnectionController(const ConnectionController &) = delete;
    void operator=(const ConnectionController &) = delete;
    ConnectionController(ConnectionController &&) = delete;
    auto operator=(ConnectionController &&) -> ConnectionController & = delete;
    ~ConnectionController() = default;

    /**
     * @brief Get the singleton instance of ServerController.
     * @details The instance will be constructed if it does not exist already.
     */
    static auto getInstance() -> ConnectionController &;

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

  private:
    ConnectionController();

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
    static void _checkConnectResult(const ProcessResult &result,
                                    int32_t countryId);

    /**
     * @brief The process ID (PID) of the process that is performing a
     * connection establishment. Has a value if < 0 while not connecting.
     */
    pid_t _connectingPid{};

    /**
     * @brief Boolean that is true while the connection is being established.
     * This time period starts with a call to #quickConnect() and ends when
     * #updateConnectionInfo() receives ConnectionStatus::Connected.
     */
    bool _quickConnecting = false;

    /**
     * @brief Implements IConnectionInfoSubscription::updateConnectionInfo().
     */
    void updateConnectionInfo(const ConnectionInfo &newInfo) override;
};

#endif // CONNECTIONCONTROLLER_HPP
