#ifndef ICONNECTIONINFOSUBSCRIPTION_HPP
#define ICONNECTIONINFOSUBSCRIPTION_HPP

/**
 * @brief IConnectionInfoSubscription is an interface that can be implemented by
 * any class that wants updates about the connection status from the
 * #StatusController. Through this interface, the observer pattern is
 * implemented.
 *
 * @details Call #StatusController::attach() and possibly
 * #StatusController::startBackgroundTasks() to start reveciving updates.
 */
class IConnectionInfoSubscription {
  public:
    /**
     * @brief Function that is called by the #StatusController every time there
     * is a #ConnectionInfo update.
     */
    virtual void updateConnectionInfo(const ConnectionInfo &newInfo) = 0;
};

#endif // ICONNECTIONINFOSUBSCRIPTION_HPP
