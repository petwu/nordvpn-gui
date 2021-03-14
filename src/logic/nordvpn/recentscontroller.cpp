#include "recentscontroller.hpp"

#include <algorithm>
#include <utility>

#include "countrycontroller.hpp"
#include "data/repositories/preferencesrepository.hpp"

RecentsController::RecentsController() {
    this->_recents = getRecentCountries();
}

auto RecentsController::getInstance() -> RecentsController & {
    static RecentsController instance;
    return instance;
}

auto RecentsController::getRecentCountries() -> std::vector<Country> {
    std::vector<Country> recents;
    std::vector<uint32_t> recentsIds =
        PreferencesRepository::getRecentCountriesIds();
    std::vector countries = CountryController::getInstance().getAllCountries();
    for (auto rId : recentsIds) {
        for (const auto &c : countries) {
            if (rId == c.id) {
                recents.push_back(c);
                break;
            }
        }
    }
    return std::move(recents);
}

void RecentsController::addTooRecentsList(uint32_t countryId) {
    PreferencesRepository::addRecentCountryId(countryId);
    this->_recents = this->getRecentCountries();
    this->notifySubscribers();
}

void RecentsController::removeFromRecentsList(uint32_t countryId) {
    PreferencesRepository::removeRecentCountryId(countryId);
    this->_recents = this->getRecentCountries();
    this->notifySubscribers();
}

void RecentsController::notifySubscriber(IRecentsSubscription &subscriber) {
    subscriber.updateRecents(this->_recents);
}
