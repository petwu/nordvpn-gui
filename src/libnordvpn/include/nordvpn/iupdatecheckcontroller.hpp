#ifndef IUPDATECONTROLLER_HPP
#define IUPDATECONTROLLER_HPP

#include "nordvpn/ibackgroundtaskable.hpp"
#include "nordvpn/isubscribable.hpp"
#include "nordvpn/models/version.hpp"
#include "nordvpn/subscriptions/iupdatechecksubscription.hpp"

/**
 * @brief The IUpdateCheckController class provides information about possible
 * updates of the application.
 */
class IUpdateCheckController
    : public virtual ISubscribable<IUpdateCheckSubscription>,
      public virtual IBackgroundTaskable {
  public:
    /**
     * @brief Returns the version of the currently running installation.
     */
    virtual auto getCurrentVersion() -> Version = 0;

    /**
     * @brief Returns the latest available version or #Version::invalid() if
     * none is available or the check failed, e.g. due to a loss of internet
     * connection or a non-successful server response.
     */
    virtual auto getLatestVersion() -> Version = 0;

    /**
     * @brief Returns the most recent non-hidden update. If the latest version
     * was not hidden using #setIsUpdateHidden(), the returned #Version is the
     * same as with #getLatestVersion(). Otherwise, this function ignores
     * available major (X.y.z) and minor (x.Y.z) releases and only returns a
     * #Version if there is an update providing a patch (x.y.Z).
     */
    virtual auto getAvailableUpdateVersion() -> Version = 0;

    /**
     * @brief Tell that the user wants to ignore a specific update version.
     */
    virtual void setIsUpdateHidden(Version v, bool isHidden) = 0;
};

#endif // IUPDATECONTROLLER_HPP
