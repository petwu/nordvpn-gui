#ifndef ISUBSCRIBABLE_HPP
#define ISUBSCRIBABLE_HPP

/**
 * @brief The ISubscribable interface allows subscribing to other classes via a
 * subscription interface.
 */
template <class TSubscription> class ISubscribable {
  public:
    /**
     * @brief Attach a subscriber that will be notified by the background task.
     * @param subscriber An object implementing TSubscription. A reasonable
     * approach is to let the class the needs the subscribed information
     * implement it itself and call attach(this).
     * @param notifyImmediately If true, the newly attached subscriber will be
     * notified initially.
     * @note Don't forget to call detach() in the destructor.
     */
    virtual void attach(TSubscription *subscriber,
                        bool notifyImmediately = false) = 0;

    /**
     * @brief Unsubscriber a subscriber. The background task will continue to
     * run.
     */
    virtual void detach(TSubscription *subscriber) = 0;
};

#endif // ISUBSCRIBABLE_HPP
