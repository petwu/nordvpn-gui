#ifndef IENVINFOSUBSCRIPTION_HPP
#define IENVINFOSUBSCRIPTION_HPP

#include "nordvpn/models/envinfo.hpp"

/**
 * @brief IEnvInfoSubscription is an interface that can be implemented by any
 * class that wants updates about the environment information collected by
 * #EnvController.
 *
 * @details Call #EnvController::attach() and possibly
 * #EnvController::startBackgroundTasks() to start reveciving updates.
 */
class IEnvInfoSubscription {
  public:
    /**
     * @brief Function that is called by the #EnvController every time there is
     * a #EnvInfo update.
     */
    virtual void updateEnv(const EnvInfo &envInfo) = 0;
};

#endif // IENVINFOSUBSCRIPTION_HPP
