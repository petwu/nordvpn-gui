#ifndef PREFERENCESREPOSITORY_H
#define PREFERENCESREPOSITORY_H

#include <vector>

#include "baserepository.h"

/**
 * @brief The PreferencesRepository class provides static functions to retieve
 * and save information related to user preferences or habits.
 */
class PreferencesRepository : public BaseRepository {
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
    static auto getRecentCountriesIds() -> std::vector<uint32_t>;

    /**
     * @brief Add a country ID to the list of countries the user recently
     * connected to.
     * @param id
     */
    static void addRecentCountryId(uint32_t id);

    /**
     * @brief Remove a country ID from the list of countries the user recently
     * connected to.
     * @param id
     */
    static void removeRecentCountryId(uint32_t id);

  private:
    /**
     * @brief _getRecentCountriesIds
     * @return
     */
    static auto _getRecentCountriesIds() -> std::vector<uint32_t>;
};

#endif // PREFERENCESREPOSITORY_H
