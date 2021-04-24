#ifndef ISTATUSCONTROLLER_HPP
#define ISTATUSCONTROLLER_HPP

#include <cstdint>

#include "nordvpn/ibackgroundtaskable.hpp"
#include "nordvpn/isubscribable.hpp"
#include "nordvpn/models/connectioninfo.hpp"
#include "nordvpn/subscriptions/iconnectioninfosubscription.hpp"

/**
 * @brief The IStatusController interface is responsible for retrieving the
 * current connection status and related information from the NordVPN CLI.
 */
class IStatusController
    : public virtual ISubscribable<IConnectionInfoSubscription>,
      public virtual IBackgroundTaskable {
  public:
    /**
     * @brief Get all information abount the current connection status collected
     * by this controller as an object.
     */
    virtual auto getStatus() -> ConnectionInfo = 0;

    /**
     * @brief Get the minimum rating value that represents the worst rating
     * possible.
     */
    virtual auto getRatingMin() -> uint8_t = 0;

    /**
     * @brief Get the maximum rating value that represents the best rating
     * possible.
     */
    virtual auto getRatingMax() -> uint8_t = 0;

    /**
     * @brief Rate NordVPNs services, especially the last connection.
     * @param rating A number representing the rating. Must be in the bounds
     * specified by getRatingMin() and getRatingMax().
     */
    virtual void rate(uint8_t rating) = 0;
};

#endif // ISTATUSCONTROLLER_HPP
