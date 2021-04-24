#ifndef IENVCONTROLLER_HPP
#define IENVCONTROLLER_HPP

#include "nordvpn/ibackgroundtaskable.hpp"
#include "nordvpn/isubscribable.hpp"
#include "nordvpn/models/envinfo.hpp"
#include "nordvpn/subscriptions/ienvinfosubscription.hpp"

/**
 * @brief The IEnvController interface is repsonsible for providing informations
 * abound the environment.
 */
class IEnvController : public virtual ISubscribable<IEnvInfoSubscription>,
                       public virtual IBackgroundTaskable {
  public:
    /**
     * @brief Get all information aboudtthe environment collected by this
     * controller as an object.
     */
    virtual auto getEnvInfo() -> EnvInfo = 0;

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
    virtual void setLoggedIn(bool loggedIn) = 0;
};

#endif // IENVCONTROLLER_HPP
