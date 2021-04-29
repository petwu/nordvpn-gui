#ifndef IUPDATECHECKSUBSCRIPTION_HPP
#define IUPDATECHECKSUBSCRIPTION_HPP

#include "nordvpn/models/version.hpp"

/**
 * @brief IUpdateCheckSubscription is an interface that can be implemented by
 * any class that wants to be notified about possibly available software updates
 * by the #IUpdateCheckController.
 *
 * @details Call #IUpdateCheckController::attach() and possibly
 * #IUpdateCheckController::startBackgroundTask() to start reveciving updates.
 */
class IUpdateCheckSubscription {
  public:
    /**
     * @brief Function that is called by the #IUpdateCheckController every time
     * there is an update on the update status.
     * @param isUpdateAvailable Whether availableVersion > currentVersion.
     * @param isUpdateHidden Whether the user requested to not show this update.
     * @param currentVersion The currently running/installed version.
     * @param availableVersion The latest non-hidden available version. May be
     * #Verison::invalid() if e.g. the update check failed.
     */
    virtual void updateUpdateStatus(bool isUpdateAvailable, bool isUpdateHidden,
                                    Version currentVersion,
                                    Version availableVersion) = 0;
};

#endif // IUPDATECHECKSUBSCRIPTION_HPP
