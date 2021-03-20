#ifndef PREFERENCESREPOSITORY_HPP
#define PREFERENCESREPOSITORY_HPP

#include <cstdint>
#include <vector>

#include "baserepository.hpp"
#include "ipreferencesrepository.hpp"

class PreferencesRepository : public IPreferencesRepository,
                              public BaseRepository {
  public:
    auto getRecentCountriesIds() -> std::vector<uint32_t> override;
    void addRecentCountryId(uint32_t id) override;
    void removeRecentCountryId(uint32_t id) override;

  private:
    /**
     * @brief _getRecentCountriesIds
     * @return
     */
    static auto _getRecentCountriesIds() -> std::vector<uint32_t>;
};

#endif // PREFERENCESREPOSITORY_HPP
