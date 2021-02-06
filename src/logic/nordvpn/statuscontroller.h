#ifndef STATUSCONTROLLER_H
#define STATUSCONTROLLER_H

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

#include "basecontroller.h"
#include "logic/models/connectioninfo.h"

/**
 * @brief Byte, the basic information unit.
 */
constexpr long Byte = 0b1;
/**
 * @brief KiB = 2 ^ 10 which equals 10 left shifts, because a left shift is the
 * same as a multiplication with the base 2.
 */
constexpr long KiB = Byte << 10;
/**  @brief MiB = 2 ^ 20  */
constexpr long MiB = KiB << 10;
/**  @brief GiB = 2 ^ 30  */
constexpr long GiB = MiB << 10;
/**  @brief TiB = 2 ^ 40  */
constexpr long TiB = GiB << 10;

/**
 * @brief IConnectionInfoSubscription is an interface that can be implemented by
 * any class that wants updates about the connection status from the
 * #StatusController. Through this interface, the observer pattern is
 * implemented.
 *
 * @details Call #StatusController::attach() and possibly
 * #StatusController::startBackgroundTask() to start reveciving updates.
 */
class IConnectionInfoSubscription {
  public:
    /**
     * @brief Function that is called by the #StatusController every time there
     * is a #ConnectionInfo update.
     */
    virtual void updateConnectionInfo(const ConnectionInfo &newInfo) = 0;
};

/**
 * @brief The StatusController class is responsible for retrieving the current
 * connection status and related information from the NordVPN CLI.
 *
 * @details The class provides a mechanism for running a background task
 * (thread) (startBackgroundTask()/stopBackgroundTask()) that periodically
 * refreshes the connection status information. The new set of information is
 * then propagated via the observer pattern. Obsersers/subscribers have the
 * implement the #IConnectionInfoSubscription interface and call attach(this) in
 * order to get notified. They can also use detach(this) to unsubscribe.
 *
 * In order to prevent multiple instances and possibly multiple background
 * tasks doing the same thing for a different set of subscribers,
 * StatusController is implemented as a singleton.
 */
class StatusController : public BaseController {
    // Singleton:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
  public:
    StatusController(const StatusController &) = delete;
    void operator=(const StatusController &) = delete;
    StatusController(StatusController &&) = delete;
    auto operator=(StatusController &&) -> StatusController & = delete;
    ~StatusController() = default;

    /**
     * @brief Get the singleton instance of StatusController.
     * @details The instance will be constructed if it does not exist already.
     */
    static auto getInstance() -> StatusController &;

    /**
     * @brief Get all information abount the current connection status collected
     * by this controller as an object.
     */
    auto getStatus() -> ConnectionInfo;

    /**
     * @brief Start a task in a background thread that periodically updates the
     * connetion status information and notifies all subscribers.
     * @see config::consts::STATUS_UPDATE_INTERVAL
     */
    void startBackgroundTask();

    /**
     * @brief Terminate the background thread.
     */
    void stopBackgroundTask();

    /**
     * @brief Attach a subscriber that will be notified by the background task.
     * @param subscriber An object implementing #IConnectionInfoSubscription.
     * The implementing class then may call attach(this).
     * @note Don't forget to call detach() in the destructor.
     */
    void attach(IConnectionInfoSubscription *subscriber);

    /**
     * @brief Unsubscriber a subscriber. The background task will continue to
     * run.
     */
    void detach(IConnectionInfoSubscription *subscriber);

    /**
     * @brief Get the minimum rating value that represents the worst rating
     * possible.
     */
    static auto getRatingMin() -> uint8_t;

    /**
     * @brief Get the maximum rating value that represents the best rating
     * possible.
     */
    static auto getRatingMax() -> uint8_t;

    /**
     * @brief Rate NordVPNs services, especially the last connection.
     * @param rating A number representing the rating. Must be in the bounds
     * specified by getRatingMin() and getRatingMax().
     */
    static void rate(uint8_t rating);

  private:
    /**
     * @brief Empty, private constructor (part of the singleton implementation).
     */
    StatusController() = default;

    /**
     * @brief Bool that is true, while the background task should keep running.
     * @details Atomic bool for thread safety.
     */
    std::atomic<bool> _performBackgroundTask = false;

    /**
     * @brief The list of subscribers.
     * @see attach(), detach()
     */
    std::vector<IConnectionInfoSubscription *> _subscribers;

    /**
     * @brief Object holding the current information about the connection
     * status.
     */
    ConnectionInfo _currentInfo;

    /**
     * @brief Object for temporarily storing connection information.
     * @details This attribute is only required to handle a special case. See
     * the comments in getStatus() for a detailed description.
     */
    ConnectionInfo _currentConnectedInfo;

    /**
     * @brief The background tasked started/stopped by
     * startBackgroundTask()/stopBackgroundTask().
     */
    void _backgroundTask();

    /**
     * @brief Notify all _subscribers.
     */
    void _notifySubscribers();

    /**
     * @brief Extract the server number from the servers hostname. Example:
     * de123.nordvpn.com --> 123
     */
    static auto _getServerNr(const std::string &name) -> int32_t;
};

#endif // STATUSCONTROLLER_H