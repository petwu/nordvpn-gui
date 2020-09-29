#ifndef PREFERENCESREPOSITORY_H
#define PREFERENCESREPOSITORY_H

#include <vector>

#include "baserepository.h"

class PreferencesRepository : public BaseRepository {
  public:
    static std::vector<uint32_t> getRecentCountriesIds();
    static void addRecentCountryId(uint32_t id);
    static void removeRecentCountryId(uint32_t id);
};

#endif // PREFERENCESREPOSITORY_H
