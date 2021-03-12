#ifndef IRECENTSSUBSCRIPTION_H
#define IRECENTSSUBSCRIPTION_H

/**
 * @brief IRecentsSubscription is an interface that can be implemented by any
 * class that wants updates of the recents list from the #RecentsController.
 *
 * @details Call #RecentsController::attach() and possibly
 * #RecentsController::startBackgroundTask() to start reveciving updates.
 */
class IRecentsSubscription {
  public:
    /**
     * @brief Function that is called by the #ServerController every time there
     * is an update of the list of recently connected countries.
     */
    virtual void updateRecents(const std::vector<Country> &newRecents) = 0;
};

#endif // IRECENTSSUBSCRIPTION_H
