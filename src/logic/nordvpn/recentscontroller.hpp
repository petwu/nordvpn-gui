#ifndef RECENTSCONTROLLER_HPP
#define RECENTSCONTROLLER_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include "basecontroller.hpp"
#include "common/templates/subscribable.hpp"
#include "data/models/country.hpp"
#include "data/repositories/ipreferencesrepository.hpp"
#include "irecentscontroller.hpp"
#include "logic/nordvpn/icountrycontroller.hpp"
#include "logic/subscriptions/irecentssubscription.hpp"

class RecentsController : public virtual IRecentsController,
                          public BaseController,
                          public Subscribable<IRecentsSubscription> {
  public:
    RecentsController(
        std::shared_ptr<ICountryController> countryController,
        std::shared_ptr<IPreferencesRepository> preferencesRepository);

    auto getRecentCountries() -> std::vector<Country> override;
    void addTooRecentsList(uint32_t countryId) override;
    void removeFromRecentsList(uint32_t countryId) override;

  private:
    const std::shared_ptr<ICountryController> _countryController;
    const std::shared_ptr<IPreferencesRepository> _preferencesRepository;

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
