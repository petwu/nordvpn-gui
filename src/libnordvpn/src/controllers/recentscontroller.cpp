#include "recentscontroller.hpp"

#include <algorithm>
#include <utility>

RecentsController::RecentsController(
    std::shared_ptr<ICountryController> countryController,
    std::shared_ptr<IPreferencesRepository> preferencesRepository)
    : _countryController(std::move(countryController)),
      _preferencesRepository(std::move(preferencesRepository)) {}

auto RecentsController::getRecentCountries() -> std::vector<Country> {
    std::vector<Country> recents;
    std::vector<uint32_t> recentsIds =
        this->_preferencesRepository->getRecentCountriesIds();
    std::vector countries = this->_countryController->getAllCountries();
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
    this->_preferencesRepository->addRecentCountryId(countryId);
    this->_recents = this->getRecentCountries();
    this->notifySubscribers();
}

void RecentsController::removeFromRecentsList(uint32_t countryId) {
    this->_preferencesRepository->removeRecentCountryId(countryId);
    this->_recents = this->getRecentCountries();
    this->notifySubscribers();
}

void RecentsController::notifySubscriber(IRecentsSubscription &subscriber) {
    if (!this->_initialGetHappened) {
        this->_initialGetHappened = false;
        this->_recents = this->getRecentCountries();
    }
    subscriber.updateRecents(this->_recents);
}
