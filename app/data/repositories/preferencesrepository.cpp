#include "preferencesrepository.h"

std::vector<uint32_t> PreferencesRepository::getRecentCountriesIds() {
    std::vector<uint32_t> recentIds;
    std::string recentsFile = readFile(config::paths::RECENTS_JSON);
    if (recentsFile == "") {
        return std::move(recentIds);
    }
    json j = json::parse(recentsFile);
    if (j.is_array()) {
        for (auto r : j) {
            if (r.is_number_integer()) {
                recentIds.push_back(json::number_integer_t(r));
            }
        }
    }
    return std::move(recentIds);
}

void PreferencesRepository::addRecentCountryId(uint32_t id) {
    std::vector<uint32_t> currentRecents = getRecentCountriesIds();
    json newRecents = json::array_t();
    newRecents.push_back(id);
    bool idIsInCurrentRecents = false;
    for (uint8_t i = 0;
         i < config::consts::MAX_RECENTS && i < currentRecents.size(); i++) {
        if (currentRecents[i] == id) {
            idIsInCurrentRecents = true;
            continue;
        }
        if (!idIsInCurrentRecents && i == config::consts::MAX_RECENTS - 1) {
            break;
        }
        newRecents.push_back(currentRecents[i]);
    }
    writeFile(config::paths::RECENTS_JSON, newRecents.dump(2));
}
