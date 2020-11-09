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

int32_t ServerController::getCountryId(std::string name) {
    for (auto i = 0; i < this->_allCountries.size(); i++) {
        auto country = this->_allCountries[i];
        std::string sName = util::string::toLower(country.name);
        std::string cName = util::string::toLower(country.connectName);
        std::string n1 = util::string::toLower(name);
        std::string n2 = util::string::replaceAll(n1, "_", " ");
        if (sName == n1 || sName == n2 || cName == n1 || cName == n2) {
            return country.id;
        }
    }
    return -1;
}

Server ServerController::getServerByHostname(std::string hostname) {
    for (auto server : this->_allServers)
        if (server.hostname == hostname)
            return server;
    return Server{};
}

std::vector<Country> ServerController::getAllCountries(bool updateFromCache) {
    if (this->_allCountries.empty() || updateFromCache) {
        auto cmdResult = Process::execute("nordvpn countries");
        auto cliCountries = util::string::split(cmdResult.output, ", ");
        std::vector<Country> all;
        if (updateFromCache)
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
            cmdResult = Process::execute("nordvpn cities " +
                                         availableCountries[i].connectName);
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

std::vector<Country> ServerController::getCountriesByGroup(Group g) {
    std::vector<Country> countries;
    for (auto server : this->_filterServerList(-1, -1)) {
        // check if the server supports the group
        // --> if no: continue
        bool containsGroup = false;
        for (auto group : server.groups) {
            if (group == g) {
                containsGroup = true;
                break;
            }
        }
        if (!containsGroup)
            continue;
        // check if the servers country already occured
        // --> if yes: continue
        bool countryAlreadyInserted = false;
        for (auto country : countries) {
            if (country.id == server.countryId) {
                countryAlreadyInserted = true;
                break;
            }
        }
        if (countryAlreadyInserted)
            continue;
        // search the country to return
        for (auto country : this->_allCountries) {
            if (country.id == server.countryId) {
                countries.push_back(country);
                break;
            }
        }
    }
    return std::move(countries);
}

std::vector<Server> ServerController::getServersByGroup(Group g,
                                                        int32_t countryId) {
    std::vector<Server> servers;
    for (auto server : this->_filterServerList(-1, -1)) {
        // check if the country ID matched
        if (countryId >= 0 && countryId != server.countryId)
            continue;
        // check if the server supports the group
        // --> if no: continue
        bool containsGroup = false;
        for (auto group : server.groups) {
            if (group == g) {
                containsGroup = true;
                break;
            }
        }
        if (!containsGroup)
            continue;
        // append
        servers.push_back(server);
    }
    return std::move(servers);
}

std::vector<Server> ServerController::_filterServerList(int32_t countryId,
                                                        int32_t cityId) {
    std::vector<Server> filtered;
    NordVpnSettings settings =
        this->_preferencesController.getNordvpnSettings();
    for (auto server : this->_allServers) {
        // skip all servers that do not meet the requirements
        if ((settings.getObfuscated().isNotNull() &&
             settings.getObfuscated().value() &&
             !server.supportsObfuscated()) ||
            (settings.getCybersec() && !server.supportsCyberSec()) ||
            !server.supportsProtocol(settings.getProtocol()) ||
            !server.supportsTechnology(settings.getTechnology()))
            continue;
        // only add servers that either match the country or city ID or both IDs
        // are unspecified
        if (server.countryId == countryId || server.cityId == cityId ||
            (countryId < 0 && cityId < 0)) {
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

void ServerController::quickConnect() { //
    AsyncProcess::execute(
        "nordvpn connect", &this->_connectingPid, [](ProcessResult result) {
            if (result.success() == false &&
                result.output.find("login") != std::string::npos) {
                EnvController::getInstance().setLoggedIn(false);
            }
        });
}

void ServerController::connectToCountryById(uint32_t id) {
    for (auto country : this->_allCountries) {
        if (country.id == id) {
            AsyncProcess::execute(
                "nordvpn connect " + country.connectName, &this->_connectingPid,
                [this, id](ProcessResult result) {
                    if (result.success() == false &&
                        result.output.find("login") != std::string::npos) {
                        EnvController::getInstance().setLoggedIn(false);
                    } else {
                        PreferencesRepository::addRecentCountryId(id);
                        this->_recents = this->getRecentCountries();
                        this->_notifySubscribersRecents();
                    }
                });
            break;
        }
    }
}

void ServerController::connectToServerById(uint32_t id) {
    for (auto server : this->_allServers) {
        if (server.id == id) {
            auto countryId = server.countryId;
            AsyncProcess::execute(
                "nordvpn connect " + server.connectName, &this->_connectingPid,
                [this, countryId](ProcessResult result) {
                    if (result.success() == false &&
                        result.output.find("login") != std::string::npos) {
                        EnvController::getInstance().setLoggedIn(false);
                    } else {
                        PreferencesRepository::addRecentCountryId(countryId);
                        this->_recents = this->getRecentCountries();
                        this->_notifySubscribersRecents();
                    }
                });
            break;
        }
    }
}

void ServerController::connectToSpecialtyGroup(Group g) {
    if (g != Group::UNDEFINED)
        AsyncProcess::execute("nordvpn connect " + group2connectName(g),
                              &this->_connectingPid);
}

void ServerController::cancelConnection() {
    // try to kill the process that is responsible for establishing the
    // connection --> might not work
    AsyncProcess::kill(this->_connectingPid, true);
    // call disconnect as an additional measure since in most cases killing
    // the connecting process does not prevent nordvpn from finishing the
    // connection establishment, but calling disconnect while connection
    // ensures that nordvpn aborts its ongoing connection operation
    this->disconnect();
}

void ServerController::disconnect() {
    AsyncProcess::execute("nordvpn disconnect");
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
        if (subscriber != nullptr)
            subscriber->updateRecents(this->_recents);
    }
}

void ServerController::_notifySubscribersCountryList() {
    for (auto &subscriber : this->_subscribers) {
        if (subscriber != nullptr)
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
        this->getAllCountries();
        this->_notifySubscribersCountryList();
    }
}
