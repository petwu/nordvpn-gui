#ifndef SUBSCRIBABLE_HPP
#define SUBSCRIBABLE_HPP

#include <algorithm>
#include <vector>

#include "nordvpn/isubscribable.hpp"

/**
 * @brief The Subscribable class is a base class for classes that allow
 * subscribing to them via a subscription interface.
 */
template <class TSubscription>
class Subscribable : public virtual ISubscribable<TSubscription> {
  public:
    void attach(TSubscription *subscriber,
                bool notifyImmediately = false) override {
        this->_subscribers.push_back(subscriber);
        if (notifyImmediately && subscriber != nullptr) {
            this->notifySubscriber(*subscriber);
        }
    }

    void detach(TSubscription *subscriber) override {
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

#endif // SUBSCRIBABLE_HPP
