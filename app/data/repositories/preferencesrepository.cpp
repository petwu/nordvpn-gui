#include "preferencesrepository.h"

std::vector<uint32_t> PreferencesRepository::getRecentCountriesIds() {
    std::vector<uint32_t> all = _getRecentCountriesIds();
    uint8_t max = config::consts::MAX_RECENTS_DISPLAY;
    uint8_t n = max <= all.size() ? max : all.size();
    std::vector<uint32_t> subset(all.begin(), all.begin() + n);
    return std::move(subset);
}

std::vector<uint32_t> PreferencesRepository::_getRecentCountriesIds() {
    std::vector<uint32_t> recentIds;
    std::string recentsFile = readFile(config::paths::RECENTS_JSON);
    if (recentsFile == "") {
        return std::move(recentIds);
    }
    json j = json::parse(recentsFile);
    if (j.is_array())
        for (auto r : j)
            if (r.is_number_integer())
                recentIds.push_back(json::number_integer_t(r));
    return std::move(recentIds);
}

void PreferencesRepository::addRecentCountryId(uint32_t id) {
    std::vector<uint32_t> currentRecents = _getRecentCountriesIds();
    json newRecents = json::array_t();
    newRecents.push_back(id);
    uint8_t max = config::consts::MAX_RECENTS_REMEMBER;
    bool idIsInCurrentRecents = false;
    for (uint8_t i = 0; i < max && i < currentRecents.size(); i++) {
        if (currentRecents[i] == id) {
            idIsInCurrentRecents = true;
            continue;
        }
        if (!idIsInCurrentRecents && i == max - 1) {
            break;
        }
        newRecents.push_back(currentRecents[i]);
    }
    writeFile(config::paths::RECENTS_JSON, newRecents.dump(2));
}

void PreferencesRepository::removeRecentCountryId(uint32_t id) {
    std::vector<uint32_t> currentRecents = _getRecentCountriesIds();
    uint8_t i;
    for (i = 0; i < currentRecents.size(); i++) {
        if (currentRecents[i] == id) {
            break;
        }
    }
    currentRecents.erase(currentRecents.begin() + i);
    json newRecents = json::array_t();
    for (auto r : currentRecents) {
        newRecents.push_back(r);
    }
    writeFile(config::paths::RECENTS_JSON, newRecents.dump(2));
}
