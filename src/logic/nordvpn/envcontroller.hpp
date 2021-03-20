#ifndef ENVCONTROLLER_HPP
#define ENVCONTROLLER_HPP

#include <string>
#include <vector>

#include "basecontroller.hpp"
#include "common/templates/backgroundtaskable.hpp"
#include "common/templates/subscribable.hpp"
#include "common/types/nullable.hpp"
#include "ienvcontroller.hpp"
#include "logic/models/envinfo.hpp"
#include "logic/subscriptions/ienvinfosubscription.hpp"

/**
 * @brief The EnvController class is repsonsible for providing informations
 * abound the environment.
 *
 * @details The class provides a mechanism for running a background task
 * (thread) (startBackgroundTasks()/stopBackgroundTasks()) that periodically
 * refreshes the environment information. The new set of information is then
 * propagated via the observer pattern. Obsersers/subscribers have the implement
 * the #IEnvInfoSubscription interface and call attach(this) in order to get
 * notified. They can also use detach(this) to unsubscribe.
 *
 * In order to prevent multiple instances and possibly multiple background
 * tasks doing the same thing for a different set of subscribers,
 * EnvController is implemented as a singleton.
 */
class EnvController : public virtual IEnvController,
                      public BaseController,
                      public Subscribable<IEnvInfoSubscription>,
                      public BackgroundTaskable {
    // Singleton:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
  public:
    EnvController(const EnvController &) = delete;
    void operator=(const EnvController &) = delete;
    EnvController(EnvController &&) = delete;
    auto operator=(EnvController &&) -> EnvController & = delete;
    ~EnvController() = default;

    /**
     * @brief Get the singleton instance of EnvController.
     * @details The instance will be constructed if it does not exist already.
     */
    static auto getInstance() -> EnvController &;

    auto getEnvInfo() -> EnvInfo override;
    void setLoggedIn(bool loggedIn) override;

  private:
    /**
     * @brief Private default constructor (part of the sigleton implementation).
     */
    EnvController();

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