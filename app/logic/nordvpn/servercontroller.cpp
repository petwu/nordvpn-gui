#include "servercontroller.h"

ServerController::ServerController() {}

std::vector<Country> ServerController::getAllCountries() {
    if (this->_allCountries.empty()) {
        auto cmdResult = execute("nordvpn countries");
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

std::vector<Server> ServerController::getServersByCountry(int32_t countryId) {
    if (this->_allServers.empty()) {
        this->_allServers = ServerRepository::fetchServers();
    }
    std::vector<Server> servers;
    for (auto server : this->_allServers) {
        if (server.countryId == countryId)
            servers.push_back(server);
    }
    return std::move(servers);
}

std::vector<Server> ServerController::getServersByCity(int32_t cityId) {
    if (this->_allServers.empty()) {
        this->_allServers = ServerRepository::fetchServers();
    }
    std::vector<Server> servers;
    for (auto server : this->_allServers) {
        if (server.cityId == cityId)
            servers.push_back(server);
    }
    return std::move(servers);
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

void ServerController::removeFromRecentsList(uint32_t countryId) {
    PreferencesRepository::removeRecentCountryId(countryId);
    this->_recents = this->getRecentCountries();
    this->_notifySubscribers();
}

void ServerController::quickConnect() {
    this->executeNonBlocking("nordvpn connect");
}

void ServerController::connectToCountryById(uint32_t id) {
    for (auto country : this->_allCountries) {
        if (country.id == id) {
            this->executeNonBlocking("nordvpn connect " + country.connectName);
            PreferencesRepository::addRecentCountryId(id);
            this->_recents = this->getRecentCountries();
            this->_notifySubscribers();
            return;
        }
    }
}

void ServerController::connectToServerById(uint32_t id) {
    for (auto server : this->_allServers) {
        if (server.id == id) {
            this->executeNonBlocking("nordvpn connect " + server.connectName);
            PreferencesRepository::addRecentCountryId(server.countryId);
            this->_recents = this->getRecentCountries();
            this->_notifySubscribers();
            return;
        }
    }
}

void ServerController::disconnect() {
    this->executeNonBlocking("nordvpn disconnect");
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
