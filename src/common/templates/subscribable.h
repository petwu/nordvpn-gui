#ifndef SUBSCRIBABLE_H
#define SUBSCRIBABLE_H

#include <algorithm>
#include <vector>

/**
 * @brief The Subscribable class is a base class for classes that allow
 * subscribing to them via a subscription interface.
 */
template <class TSubscription> class Subscribable {
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
    void attach(TSubscription *subscriber, bool notifyImmediately = false) {
        this->_subscribers.push_back(subscriber);
        if (notifyImmediately && subscriber != nullptr) {
            this->notifySubscribers();
        }
    }

    /**
     * @brief Unsubscriber a subscriber. The background task will continue to
     * run.
     */
    void detach(TSubscription *subscriber) {
        this->_subscribers.erase(std::remove(this->_subscribers.begin(),
                                             this->_subscribers.end(),
                                             subscriber),
                                 this->_subscribers.end());
    }

  protected:
    /**
     * @brief Notify all subscribers. May be overridden.
     */
    virtual void notifySubscribers() {
        for (auto &subscriber : this->_subscribers) {
            if (subscriber != nullptr) {
                this->notifySubscriber(*subscriber);
            }
        }
    }

    /**
     * @brief Callback that is responsible for notifying a single subscriber.
     * @param subscriber The subscription/subscriber.
     * @note Null-checking the subscription is not required.
     */
    virtual void notifySubscriber(TSubscription &subscriber) = 0;

  private:
    /**
     * @brief The list of subscribers.
     * @see attach(), detach()
     */
    std::vector<TSubscription *> _subscribers;
};

#endif // SUBSCRIBABLE_H
