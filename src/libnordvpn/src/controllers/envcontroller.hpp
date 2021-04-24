#ifndef ENVCONTROLLER_HPP
#define ENVCONTROLLER_HPP

#include <string>
#include <vector>

#include "backgroundtaskable.hpp"
#include "basecontroller.hpp"
#include "nordvpn/ienvcontroller.hpp"
#include "nordvpn/models/envinfo.hpp"
#include "nordvpn/models/nullable.hpp"
#include "nordvpn/subscriptions/ienvinfosubscription.hpp"
#include "subscribable.hpp"

class EnvController : public virtual IEnvController,
                      public BaseController,
                      public Subscribable<IEnvInfoSubscription>,
                      public BackgroundTaskable {
  public:
    EnvController();

    auto getEnvInfo() -> EnvInfo override;
    void setLoggedIn(bool loggedIn) override;

  private:
    /**
     * @brief The current set of environment informations.
     */
    EnvInfo _envInfo;

    /**
     * @brief Implementation of Subscribable::notifySubscriber().
     */
    void notifySubscriber(IEnvInfoSubscription &subscriber) override;

    /**
     * @brief The background task for periodically re-fetch all information and
     * notify subscribers about it.
     */
    void _backgroundTask(bool isSpecialTick);

    /**
     * @brief One-time initialization of _backgroundTask.
     */
    void _backgroundTaskInit();

    /**
     * @brief Checks whether the devices has an active internet connection.
     * @details Should be executed in a detached background thread.
     */
    static auto _isInternetConnected() -> bool;

    /**
     * @brief Checks whether the devices allows the execution of shell commands
     * (`/bin/sh -c "command"`). This is essential for making calls to the
     * NordVPN CLI.
     */
    static auto _isShellAvailable() -> bool;

    /**
     * @brief Checks whether the NordVPN CLI (`nordvpn`) is installed and
     * available in PATH.
     */
    static auto _isNordvpnInstalled() -> bool;

    /**
     * @brief Checks whether the a user is logged in to the NordVPN CLI.
     */
    static auto _isLoggedIn() -> Nullable<bool>;

    /**
     * @brief Call "nordvpn status" to check if it returns any error that cannot
     * be narrowed down any further.
     * @return An empty string in case everything is alright.
     */
    static auto _checkMiscError() -> std::string;
};

#endif // ENVCONTROLLER_HPP
