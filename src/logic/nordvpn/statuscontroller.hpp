#ifndef STATUSCONTROLLER_HPP
#define STATUSCONTROLLER_HPP

#include <atomic>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

#include "basecontroller.hpp"
#include "common/templates/backgroundtaskable.hpp"
#include "common/templates/subscribable.hpp"
#include "istatuscontroller.hpp"
#include "logic/models/connectioninfo.hpp"
#include "logic/subscriptions/iconnectioninfosubscription.hpp"

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

class StatusController : public virtual IStatusController,
                         public BaseController,
                         public Subscribable<IConnectionInfoSubscription>,
                         public BackgroundTaskable {
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

    auto getStatus() -> ConnectionInfo override;
    auto getRatingMin() -> uint8_t override;
    auto getRatingMax() -> uint8_t override;
    void rate(uint8_t rating) override;

  private:
    /**
     * @brief Private constructor (part of the singleton implementation).
     */
    StatusController();

    /**
     * @brief Bool that is true, while the background task should keep running.
     * @details Atomic bool for thread safety.
     */
    std::atomic<bool> _performBackgroundTask = false;

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
     * @brief The background task that is run periodically to fetch/update the
     * latest status.
     */
    void _backgroundTask();

    /**
     * @brief Implementation of Subscribable::notifySubscriber().
     */
    void notifySubscriber(IConnectionInfoSubscription &subscriber) override;

    /**
     * @brief Extract the server number from the servers hostname. Example:
     * de123.nordvpn.com --> 123
     */
    static auto _getServerNr(const std::string &name) -> int32_t;
};

#endif // STATUSCONTROLLER_HPP
