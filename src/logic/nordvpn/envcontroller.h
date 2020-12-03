#ifndef ENVCONTROLLER_H
#define ENVCONTROLLER_H

#include "curl/curl.h"

#include "basecontroller.h"
#include "logic/models/envinfo.h"

/**
 * @brief IEnvInfoSubscription is an interface that can be implemented by any
 * class that wants updates about the environment information collected by
 * #EnvController.
 *
 * @details Call #EnvController::attach() and possibly
 * #EnvController::startBackgroundTask() to start reveciving updates.
 */
class IEnvInfoSubscription {
  public:
    /**
     * @brief Function that is called by the #EnvController every time there is
     * a #EnvInfo update.
     */
    virtual void updateEnv(const EnvInfo &envInfo) = 0;
};

/**
 * @brief The EnvController class is repsonsible for providing informations
 * abound the environment.
 *
 * @details The class provides a mechanism for running a background task
 * (thread) (startBackgroundTask()/stopBackgroundTask()) that periodically
 * refreshes the environment information. The new set of information is then
 * propagated via the observer pattern. Obsersers/subscribers have the implement
 * the #IEnvInfoSubscription interface and call attach(this) in order to get
 * notified. They can also use detach(this) to unsubscribe.
 *
 * In order to prevent multiple instances and possibly multiple background
 * tasks doing the same thing for a different set of subscribers,
 * EnvController is implemented as a singleton.
 */
class EnvController : public BaseController {
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

    /**
     * @brief Get all information aboudtthe environment collected by this
     * controller as an object.
     */
    static auto getEnvInfo() -> EnvInfo;

    /**
     * @brief Force an override of the "logged in"-information.
     * @details Use this function direclty after the users logged in/out over
     * this application. All subscribers will be notified about this change.
     *
     * Why this function?: The "logged in" information will be updated anyway by
     * the background task, but with this function the subscribers get
     * notified immediately and don't have to wait until the next background
     * task intervall.
     */
    void setLoggedIn(bool loggedIn);

    /**
     * @brief Start a task in a background thread that periodically updates the
     * environment information and notifies all subscribers.
     * @see config::consts::ENV_UPDATE_INTERVAL
     */
    void startBackgroundTask();

    /**
     * @brief Terminate the background thread.
     */
    void stopBackgroundTask();

    /**
     * @brief Attach a subscriber that will be notified by the background task.
     * @param subscriber An object implementing #IEnvInfoSubscription. A
     * reasonable approach is to let the class the needs the environment
     * information implement it itself and call attach(this).
     * @note Don't forget to call detach() in the destructor.
     */
    void attach(IEnvInfoSubscription *subscriber);

    /**
     * @brief Unsubscriber a subscriber. The background task will continue to
     * run.
     */
    void detach(IEnvInfoSubscription *subscriber);

  private:
    /**
     * @brief Empty constructor (part of the sigleton implementation).
     */
    EnvController() = default;

    /**
     * @brief The current set of environment informations.
     */
    EnvInfo _envInfo;

    /**
     * @brief The list of subscribers.
     * @see attach(), detach()
     */
    std::vector<IEnvInfoSubscription *> _subscribers;

    /**
     * @brief Notify all _subscribers.
     */
    void _notifySubscribers();

    /**
     * @brief Bool that is true, while the background task should keep running.
     * @details Atomic bool for thread safety.
     */
    std::atomic<bool> _performBackgroundTask = false;

    /**
     * @brief The background tasked started/stopped by
     * startBackgroundTask()/stopBackgroundTask().
     */
    void _backgroundTask();

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
    static auto _isLoggedIn() -> bool;
};

#endif // ENVCONTROLLER_H
