#ifndef IRECENTSCONTROLLER_HPP
#define IRECENTSCONTROLLER_HPP

#include <cstdint>
#include <vector>

#include "nordvpn/isubscribable.hpp"
#include "nordvpn/models/country.hpp"
#include "nordvpn/subscriptions/irecentssubscription.hpp"

/**
 * @brief The IRecentsController interface is responsible for managing a list of
 * recent connections.
 */
class IRecentsController : public virtual ISubscribable<IRecentsSubscription> {
  public:
    /**
     * @brief Get a list of all countries available.
     */
    virtual auto getRecentCountries() -> std::vector<Country> = 0;

    /**
     * @brief Add a country to the list of recently connected countries by ID.
     */
    virtual void addTooRecentsList(uint32_t countryId) = 0;

    /**
     * @brief Remove a country from the list of recently connected countries by
     * ID.
     */
    virtual void removeFromRecentsList(uint32_t countryId) = 0;
};

#endif // IRECENTSCONTROLLER_HPP
