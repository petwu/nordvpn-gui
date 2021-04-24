#ifndef ICOUNTRYCONTROLLER_H
#define ICOUNTRYCONTROLLER_H

#include <cstdint>
#include <vector>

#include "nordvpn/enums/group.hpp"
#include "nordvpn/ibackgroundtaskable.hpp"
#include "nordvpn/isubscribable.hpp"
#include "nordvpn/models/country.hpp"
#include "nordvpn/models/location.hpp"
#include "nordvpn/models/nullable.hpp"
#include "nordvpn/subscriptions/icountriessubscription.hpp"

/**
 * @brief The ICountryController interface provides functions to retrieve
 * information about countries and cities.
 */
class ICountryController
    : public virtual IBackgroundTaskable,
      public virtual ISubscribable<ICountriesSubscription> {
  public:
    /**
     * @brief Get a list of all countries.
     * @details The list of countries is cached on the file system to be able to
     * return it faster and to reduce the amount of API requests required. The
     * `updateCache` parameter can be used to force an API request and
     * override the cached list. This should be done rarely (like once per
     * application start). In cache `updateCache` is false and no cached list
     * exists yet, the API request will be performed automatically.
     */
    virtual auto getAllCountries(bool updateCache = false)
        -> std::vector<Country> = 0;

    /**
     * @brief Returns a specific #Country object by ID.
     */
    virtual auto getCountryById(uint32_t id) -> Nullable<Country> = 0;

    /**
     * @brief Returns a specific #Country object that contains a city ID.
     */
    virtual auto getCountryByCityId(uint32_t id) -> Nullable<Country> = 0;

    /**
     * @brief Returns a specific city as #Location object by ID.
     */
    virtual auto getCityById(uint32_t id) -> Nullable<Location> = 0;

    /**
     * @brief Returns all countries that are belong to a specific server group.
     */
    virtual auto getCountriesByGroup(Group g) -> std::vector<Country> = 0;
};

#endif // ICOUNTRYCONTROLLER_H
