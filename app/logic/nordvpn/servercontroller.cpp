#include "servercontroller.h"

std::vector<Country> ServerController::getAllCountries() {
    if (this->_allCountries.empty()) {
        auto cmdResult = execute(config::cmd::COUTRIES);
        auto availableCountries = util::string::split(cmdResult.output, ", ");
        std::vector<Country> all = ServerRepository::fetchCountries();
        std::vector<Country> available;
        // filter the vector to only contains countries that were returned
        // by the CLI command
        for (std::string c : availableCountries) {
            for (auto e : all) {
                if (c == e.connectName) {
                    available.push_back(e);
                }
            }
        }
        this->_allCountries = available;
    }
    return this->_allCountries;
}

std::vector<std::string> ServerController::getCities(std::string country) {
    auto result = execute(config::cmd::CITIES + " " + country);
    return util::string::split(result.output, ", ");
}

std::vector<std::string> ServerController::getGroups() {
    auto result = execute(config::cmd::GROUPS);
    return util::string::split(result.output, ", ");
}

std::vector<Country> ServerController::getRecentCountries() {
    std::vector<Country> recents;
    std::vector<uint32_t> recentsIds =
        PreferencesRepository::getRecentCountriesIds();
    std::vector countries = this->getAllCountries();
    for (auto rId : recentsIds) {
        for (auto c : countries) {
            if (rId == c.id) {
                recents.push_back(c);
                break;
            }
        }
    }
    return std::move(recents);
}

void ServerController::quickConnect() {
    this->executeNonBlocking(config::cmd::CONNECT);
}

void ServerController::connectToCountryById(uint32_t id) {
    for (auto country : this->_allCountries) {
        if (country.id == id) {
            this->executeNonBlocking(config::cmd::CONNECT + " " +
                                     std::string(country.connectName));
            PreferencesRepository::addRecentCountryId(id);
            this->_recents = this->getRecentCountries();
            this->_notifySubscribers();
            return;
        }
    }
}

void ServerController::disconnect() {
    this->executeNonBlocking(config::cmd::DISCONNECT);
}

void ServerController::attach(IRecentCountriesSubscription *subscriber) {
    this->_subscribers.push_back(subscriber);
}

void ServerController::detach(IRecentCountriesSubscription *subscriber) {
    this->_subscribers.erase(std::remove(this->_subscribers.begin(),
                                         this->_subscribers.end(), subscriber),
                             this->_subscribers.end());
}

void ServerController::_notifySubscribers() {
    for (auto &subscriber : this->_subscribers) {
        subscriber->updateRecents(this->_recents);
    }
}
