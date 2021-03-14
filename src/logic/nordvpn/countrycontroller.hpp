#ifndef COUNTRYCONTROLLER_HPP
#define COUNTRYCONTROLLER_HPP

#include <cstdint>
#include <vector>

#include "common/templates/backgroundtaskable.hpp"
#include "common/templates/subscribable.hpp"
#include "common/types/nullable.hpp"
#include "data/enums/group.hpp"
#include "data/models/country.hpp"
#include "data/models/location.hpp"
#include "logic/subscriptions/icountriessubscription.hpp"

class CountryController : public BackgroundTaskable,
                          public Subscribable<ICountriesSubscription> {
    // Singleton:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
  public:
    CountryController(const CountryController &) = delete;
    void operator=(const CountryController &) = delete;
    CountryController(CountryController &&) = delete;
    auto operator=(CountryController &&) -> CountryController & = delete;
    ~CountryController() = default;

    /**
     * @brief Get the singleton instance of CountryController.
     * @details The instance will be constructed if it does not exist already.
     */
    static auto getInstance() -> CountryController &;

    /**
     * @brief Get a list of all countries.
     * @details The list of countries is cached on the file system to be able to
     * return it faster and to reduce the amount of API requests required. The
     * `updateCache` parameter can be used to force an API request and
     * override the cached list. This should be done rarely (like once per
     * application start). In cache `updateCache` is false and no cached list
     * exists yet, the API request will be performed automatically.
     */
    auto getAllCountries(bool updateCache = false) -> std::vector<Country>;

    /**
     * @brief Returns a specific #Country object by ID.
     */
    auto getCountryById(uint32_t id) -> Nullable<Country>;

    /**
     * @brief Returns a specific #Country object that contains a city ID.
     */
    auto getCountryByCityId(uint32_t id) -> Nullable<Country>;

    /**
     * @brief Returns a specific city as #Location object by ID.
     */
    auto getCityById(uint32_t id) -> Nullable<Location>;

    /**
     * @brief Returns all countries that are belong to a specific server group.
     */
    auto getCountriesByGroup(Group g) -> std::vector<Country>;

  private:
    /**
     * @brief Empty, private constructor (part of the sigleton implementation).
     */
    CountryController();

    /**
     * @brief Internal list of all countries.
     */
    std::vector<Country> _allCountries;

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
