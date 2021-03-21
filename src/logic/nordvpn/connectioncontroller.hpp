#ifndef CONNECTIONCONTROLLER_HPP
#define CONNECTIONCONTROLLER_HPP

#include <cstdint>
#include <memory>
#include <sys/types.h>

#include "common/io/processresult.hpp"
#include "data/enums/group.hpp"
#include "iconnectioncontroller.hpp"
#include "logic/models/connectioninfo.hpp"
#include "logic/nordvpn/icountrycontroller.hpp"
#include "logic/nordvpn/ienvcontroller.hpp"
#include "logic/nordvpn/irecentscontroller.hpp"
#include "logic/nordvpn/iservercontroller.hpp"
#include "logic/nordvpn/istatuscontroller.hpp"
#include "logic/subscriptions/iconnectioninfosubscription.hpp"

class ConnectionController : public virtual IConnectionController,
                             public IConnectionInfoSubscription {
  public:
    ConnectionController(std::shared_ptr<ICountryController> countryController,
                         std::shared_ptr<IEnvController> envController,
                         std::shared_ptr<IRecentsController> recentsController,
                         std::shared_ptr<IServerController> serverController,
                         std::shared_ptr<IStatusController> statusController);

    void quickConnect() override;
    void connectToCountryById(uint32_t id) override;
    void connectToCityById(uint32_t id) override;
    void connectToServerById(uint32_t id) override;
    void connectToSpecialtyGroup(Group g) override;
    void connectToCountryByIdAndGroup(uint32_t id, Group g) override;
    void cancelConnection() override;
    void disconnect() override;

  private:
    const std::shared_ptr<ICountryController> _countryController;
    const std::shared_ptr<IEnvController> _envController;
    const std::shared_ptr<IRecentsController> _recentsController;
    const std::shared_ptr<IServerController> _serverController;
    const std::shared_ptr<IStatusController> _statusController;

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
