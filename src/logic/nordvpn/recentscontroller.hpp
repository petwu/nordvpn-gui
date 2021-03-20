#ifndef RECENTSCONTROLLER_HPP
#define RECENTSCONTROLLER_HPP

#include <cstdint>
#include <vector>

#include "basecontroller.hpp"
#include "common/templates/subscribable.hpp"
#include "data/models/country.hpp"
#include "data/repositories/preferencesrepository.hpp"
#include "irecentscontroller.hpp"
#include "logic/subscriptions/irecentssubscription.hpp"

class RecentsController : public virtual IRecentsController,
                          public BaseController,
                          public Subscribable<IRecentsSubscription> {
    // Singleton:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
  public:
    RecentsController(const RecentsController &) = delete;
    void operator=(const RecentsController &) = delete;
    RecentsController(RecentsController &&) = delete;
    auto operator=(RecentsController &&) -> RecentsController & = delete;
    ~RecentsController() = default;

    /**
     * @brief Get the singleton instance of RecentsController.
     * @details The instance will be constructed if it does not exist already.
     */
    static auto getInstance() -> RecentsController &;

    auto getRecentCountries() -> std::vector<Country> override;
    void addTooRecentsList(uint32_t countryId) override;
    void removeFromRecentsList(uint32_t countryId) override;

  private:
    RecentsController() = default;

    PreferencesRepository _preferencesRepository;

    /**
     * @brief Internal list of all recently connected countries.
     */
    std::vector<Country> _recents;

    /**
     * @brief Flag to perform an initial #getRecentCountries() before notifiying
     * subscribers.
     */
    bool _initialGetHappened = false;

    /**
     * @brief Implementation of Subscribable::notifySubscriber().
     * @param subscriber
     */
    void notifySubscriber(IRecentsSubscription &subscriber) override;
};

#endif // RECENTSCONTROLLER_HPP
