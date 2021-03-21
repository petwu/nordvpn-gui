#ifndef COUNTRYCONTROLLER_HPP
#define COUNTRYCONTROLLER_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include "common/templates/backgroundtaskable.hpp"
#include "common/templates/subscribable.hpp"
#include "common/types/nullable.hpp"
#include "data/enums/group.hpp"
#include "data/models/country.hpp"
#include "data/models/location.hpp"
#include "data/repositories/iserverrepository.hpp"
#include "icountrycontroller.hpp"
#include "logic/nordvpn/iservercontroller.hpp"
#include "logic/subscriptions/icountriessubscription.hpp"

class CountryController : public virtual ICountryController,
                          public BackgroundTaskable,
                          public Subscribable<ICountriesSubscription> {
  public:
    CountryController(std::shared_ptr<IServerController> serverController,
                      std::shared_ptr<IServerRepository> serverRepository);

    auto getAllCountries(bool updateCache = false)
        -> std::vector<Country> override;
    auto getCountryById(uint32_t id) -> Nullable<Country> override;
    auto getCountryByCityId(uint32_t id) -> Nullable<Country> override;
    auto getCityById(uint32_t id) -> Nullable<Location> override;
    auto getCountriesByGroup(Group g) -> std::vector<Country> override;

  private:
    const std::shared_ptr<IServerController> _serverController;
    const std::shared_ptr<IServerRepository> _serverRepository;

    /**
     * @brief Internal list of all countries.
     */
    std::vector<Country> _allCountries;

    /**
     * @brief Backing implementation method of #getAllCountries().
     * @details Requied to use this method in the constructor instead of the
     * virtual interface implementation. Reason:
     *
     * ```
     * warning: Call to virtual method 'RecentsController::getRecentCountries'
     * during construction bypasses virtual dispatch
     * [clang-analyzer-optin.cplusplus.VirtualCall]
     * ```
     */
    auto _getAllCountries(bool updateCache = false) -> std::vector<Country>;

    /**
     * @brief The background task responsible for periodically updating the
     * country list.
     */
    void _backgroundTask(bool isSpecialTick);

    /**
     * @brief Implementation of Subscribable::notifySubscriber().
     * @param subscriber
     */
    void notifySubscriber(ICountriesSubscription &subscriber) override;
};

#endif // COUNTRYCONTROLLER_HPP
