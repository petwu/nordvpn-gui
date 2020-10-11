#include "servercontroller.h"

ServerController::ServerController() {
    // use a cached server list which is stored on disk and can be read in fast
    // so that the UI has something do display
    this->_allServers = ServerRepository::fetchServersFromCache();
    // then fetch the up to date server list from the NordVPN API in a
    // background thread since the complete server list is a JSON file with a
    // size of ~16 MiB (as of 2020-10-10) and hence might take some time to
    // download and parse
    std::thread([this] {
        this->_allServers = ServerRepository::fetchServers();
    }).detach();
}

ServerController &ServerController::getInstance() {
    static ServerController instance;
    return instance;
}

std::vector<Country> ServerController::getAllCountries(bool updateFromCache) {
    if (this->_allCountries.empty() || updateFromCache) {
        auto cmdResult = execute("nordvpn countries");
        auto cliCountries = util::string::split(cmdResult.output, ", ");
        std::vector<Country> all;
        if (updateFromCache && !this->_allCountries.empty())
            all = ServerRepository::fetchCountriesFromCache();
        else
            all = ServerRepository::fetchCountries();
        std::vector<Country> availableCountries;
        // filter the vector to only contains countries that were returned
        // by the CLI command
        for (std::string c : cliCountries) {
            for (auto e : all) {
                if (c == e.connectName) {
                    availableCountries.push_back(e);
                }
            }
        }
        // filter the countries to only contain cities that were returned by the
        // CLI command
        for (size_t i = 0; i < availableCountries.size(); i++) {
            cmdResult =
                execute("nordvpn cities " + availableCountries[i].connectName);
            auto cliCities = util::string::split(cmdResult.output, ", ");
            std::vector<Location> availableCities;
            for (auto city : availableCountries[i].cities) {
                for (auto cliCity : cliCities) {
                    if (Connectable::fuzzyMatchNames(city.name, cliCity)) {
                        availableCities.push_back(city);
                    }
                }
            }
            availableCountries[i].cities = availableCities;
        }
        this->_allCountries = availableCountries;
    }
    return this->_allCountries;
}

std::vector<Server> ServerController::getServersByCountry(int32_t countryId) {
    return this->_filterServerList(countryId, -1);
}

std::vector<Server> ServerController::getServersByCity(int32_t cityId) {
    return this->_filterServerList(-1, cityId);
}

std::vector<Server> ServerController::_filterServerList(int32_t countryId,
                                                        int32_t cityId) {
    std::vector<Server> filtered;
    NordVpnSettings settings = this->_settingsController.getSettings();
    for (auto server : this->_allServers) {
        // skip all servers that do not meet the requirements
        if ((settings.obfuscated && !server.supportsObfuscated()) ||
            (settings.cybersec && !server.supportsCyberSec()) ||
            !server.supportsProtocol(settings.protocol) ||
            !server.supportsTechnology(settings.technology))
            continue;
        // only add servers that either match the country or city ID
        if (server.countryId == countryId || server.cityId == cityId) {
            filtered.push_back(server);
        }
    }
    return std::move(filtered);
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
    this->_notifySubscribersRecents();
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
            this->_notifySubscribersRecents();
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
            this->_notifySubscribersRecents();
            return;
        }
    }
}

void ServerController::disconnect() {
    this->executeNonBlocking("nordvpn disconnect");
}

void ServerController::attach(ICountriesSubscription *subscriber) {
    this->_subscribers.push_back(subscriber);
}

void ServerController::detach(ICountriesSubscription *subscriber) {
    this->_subscribers.erase(std::remove(this->_subscribers.begin(),
                                         this->_subscribers.end(), subscriber),
                             this->_subscribers.end());
}

void ServerController::_notifySubscribersRecents() {
    for (auto &subscriber : this->_subscribers) {
        subscriber->updateRecents(this->_recents);
    }
}

void ServerController::_notifySubscribersCountryList() {
    for (auto &subscriber : this->_subscribers) {
        subscriber->updateCountryList(this->_allCountries);
    }
}

void ServerController::startBackgroundTask() {
    this->_performBackgroundTask = true;
    // create and run new daemon threads
    std::thread(&ServerController::_backgroundTaskServerList, this).detach();
    std::thread(&ServerController::_backgroundTaskCountryList, this).detach();
}

void ServerController::stopBackgroundTask() {
    this->_performBackgroundTask = false;
}

void ServerController::_backgroundTaskServerList() {
    while (this->_performBackgroundTask) {
        std::this_thread::sleep_for(
            config::consts::SERVER_LIST_UPDATE_INTERVAL);
        this->_allServers = ServerRepository::fetchServers();
    }
}

void ServerController::_backgroundTaskCountryList() {
    while (this->_performBackgroundTask) {
        std::this_thread::sleep_for(
            config::consts::COUNTRY_LIST_UPDATE_INTERVAL);
        this->getAllCountries(true);
        this->_notifySubscribersCountryList();
    }
}
