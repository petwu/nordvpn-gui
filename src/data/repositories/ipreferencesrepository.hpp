#ifndef IPREFERENCESREPOSITORY_HPP
#define IPREFERENCESREPOSITORY_HPP

#include <cstdint>
#include <vector>

/**
 * @brief The IPreferencesRepository interface provides functions to retieve
 * and save information related to user preferences or habits.
 */
class IPreferencesRepository {
  public:
    /**
     * @brief Get a list of country IDs the user recently connected to. The
     * returned list is limited to a smaller length that the list stored on the
     * filesystem. This means in the following code `before` and `after` may
     * have the same size.
     *
     * \code{.cpp}
     * auto before = getRecentCountriesIds();
     * removeRecentCountryId(123);
     * auto after = getRecentCountriesIds();
     * \endcode
     *
     * @return
     */
    virtual auto getRecentCountriesIds() -> std::vector<uint32_t> = 0;

    /**
     * @brief Add a country ID to the list of countries the user recently
     * connected to.
     * @param id
     */
    virtual void addRecentCountryId(uint32_t id) = 0;

    /**
     * @brief Remove a country ID from the list of countries the user recently
     * connected to.
     * @param id
     */
    virtual void removeRecentCountryId(uint32_t id) = 0;
};
#endif // IPREFERENCESREPOSITORY_HPP
