#include "servercontroller.h"

#include <algorithm>
#include <chrono>
#include <thread>
#include <utility>

#include "common/io/asyncprocess.h"
#include "common/io/process.h"
#include "common/types/nullable.h"
#include "common/util/strings.h"
#include "config.h"
#include "data/enums/securityprotocol.h"
#include "data/models/connectable.h"
#include "data/models/location.h"
#include "data/repositories/preferencesrepository.h"
#include "data/repositories/serverrepository.h"
#include "logic/enums/connectionstatus.h"
#include "logic/models/nordvpnsettings.h"
#include "logic/nordvpn/envcontroller.h"
#include "logic/nordvpn/preferencescontroller.h"

ServerController::ServerController() {
    // register my background tasks
    this->registerBackgroundTask(
        std::bind(&ServerController::_backgroundTaskServerList, this),
        config::consts::SERVER_LIST_UPDATE_INTERVAL);
    this->registerBackgroundTask(
        std::bind(&ServerController::_backgroundTaskCountryList, this, _1),
        config::consts::COUNTRY_LIST_UPDATE_INTERVAL, std::chrono::hours(24));
    // subscribe to connection status updates
    StatusController::getInstance().attach(this);
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

auto ServerController::getInstance() -> ServerController & {
    static ServerController instance;
    return instance;
}

auto ServerController::getServerByHostname(const std::string &hostname)
    -> Server {
    for (auto server : this->_allServers) {
        if (server.hostname == hostname) {
            return server;
        }
    }
    return Server{};
}

auto ServerController::getAllCountries(bool updateCache)
    -> std::vector<Country> {
    if (this->_allCountries.empty() || updateCache) {
        auto cmdResult = Process::execute("nordvpn countries");
        auto cliCountries = util::string::split(cmdResult.output, ", ");
        std::vector<Country> all;
        if (updateCache) {
            all = ServerRepository::fetchCountries();
        } else {
            all = ServerRepository::fetchCountriesFromCache();
        }
        std::vector<Country> availableCountries;
        // filter the vector to only contains countries that were returned
        // by the CLI command
        for (const std::string &c : cliCountries) {
            for (const auto &e : all) {
                if (c == e.connectName) {
                    availableCountries.push_back(e);
                }
            }
        }
        // filter the countries to only contain cities that were returned by the
        // CLI command
        for (auto &availableCountrie : availableCountries) {
            cmdResult = Process::execute("nordvpn cities " +
                                         availableCountrie.connectName);
            auto cliCities = util::string::split(cmdResult.output, ", ");
            std::vector<Location> availableCities;
            for (const auto &city : availableCountrie.cities) {
                for (const auto &cliCity : cliCities) {
                    if (Connectable::fuzzyMatchNames(city.name, cliCity)) {
                        availableCities.push_back(city);
                    }
                }
            }
            availableCountrie.cities = availableCities;
        }
        this->_allCountries = availableCountries;
    }
    return this->_allCountries;
}

auto ServerController::getServersByCountry(int32_t countryId)
    -> std::vector<Server> {
    return this->_filterServerList(countryId, -1);
}

auto ServerController::getServersByCity(int32_t cityId) -> std::vector<Server> {
    return this->_filterServerList(-1, cityId);
}

auto ServerController::getCountriesByGroup(Group g) -> std::vector<Country> {
    std::vector<Country> countries;
    for (const auto &server : this->_filterServerList(-1, -1)) {
        // check if the server supports the group
        // --> if no: continue
        bool containsGroup = false;
        for (auto group : server.groups) {
            if (group == g) {
                containsGroup = true;
                break;
            }
        }
        if (!containsGroup) {
            continue;
        }
        // check if the servers country already occured
        // --> if yes: continue
        bool countryAlreadyInserted = false;
        for (const auto &country : countries) {
            if (country.id == server.countryId) {
                countryAlreadyInserted = true;
                break;
            }
        }
        if (countryAlreadyInserted) {
            continue;
        }
        // search the country to return
        for (const auto &country : this->_allCountries) {
            if (country.id == server.countryId) {
                countries.push_back(country);
                break;
            }
        }
    }
    return std::move(countries);
}

auto ServerController::getServersByGroup(Group g, int32_t countryId)
    -> std::vector<Server> {
    std::vector<Server> servers;
    for (const auto &server : this->_filterServerList(-1, -1)) {
        // check if the country ID matched
        if (countryId >= 0 && countryId != server.countryId) {
            continue;
        }
        // check if the server supports the group
        // --> if no: continue
        bool containsGroup = false;
        for (auto group : server.groups) {
            if (group == g) {
                containsGroup = true;
                break;
            }
        }
        if (!containsGroup) {
            continue;
        }
        // append
        servers.push_back(server);
    }
    return std::move(servers);
}

auto ServerController::_filterServerList(int32_t countryId, int32_t cityId)
    -> std::vector<Server> {
    std::vector<Server> filtered;
    NordVpnSettings settings = PreferencesController::getNordvpnSettings();
    for (auto server : this->_allServers) {
        // skip all servers that do not meet the requirements
        if ((settings.getObfuscated().isNotNull() &&
             settings.getObfuscated().value() &&
             !server.supportsObfuscated()) ||
            (settings.getCybersec() && !server.supportsCyberSec()) ||
            !server.supportsProtocol(settings.getProtocol()) ||
            !server.supportsTechnology(settings.getTechnology())) {
            continue;
        }
        // only add servers that either match the country or city ID or both IDs
        // are unspecified
        if (server.countryId == countryId || server.cityId == cityId ||
            (countryId < 0 && cityId < 0)) {
            filtered.push_back(server);
        }
    }
    return std::move(filtered);
}

auto ServerController::getRecentCountries() -> std::vector<Country> {
    std::vector<Country> recents;
    std::vector<uint32_t> recentsIds =
        PreferencesRepository::getRecentCountriesIds();
    std::vector countries = this->getAllCountries();
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

void ServerController::removeFromRecentsList(uint32_t countryId) {
    PreferencesRepository::removeRecentCountryId(countryId);
    this->_recents = this->getRecentCountries();
    this->_notifySubscribersRecents();
}

void ServerController::quickConnect() { //
    this->_quickConnecting = true;
    AsyncProcess::execute(
        "nordvpn connect", &this->_connectingPid,
        [this](const ProcessResult &result) {
            if (!result.success() &&
                result.output.find("login") != std::string::npos) {
                this->_quickConnecting = false;
                EnvController::getInstance().setLoggedIn(false);
            }
        });
}

void ServerController::connectToCountryById(uint32_t id) {
    for (const auto &country : this->_allCountries) {
        if (country.id == id) {
            AsyncProcess::execute("nordvpn connect " + country.connectName,
                                  &this->_connectingPid,
                                  [this, id](const ProcessResult &result) {
                                      this->_checkConnectResult(result, id);
                                  });
            break;
        }
    }
}

void ServerController::connectToCityById(uint32_t id) {
    bool found = false;
    for (const auto &country : this->_allCountries) {
        for (const auto &city : country.cities) {
            if (city.id == id) {
                found = true;
                auto countryId = country.id;
                AsyncProcess::execute(
                    "nordvpn connect " + country.connectName + " " +
                        city.connectName,
                    &this->_connectingPid,
                    [this, countryId](const ProcessResult &result) { //
                        this->_checkConnectResult(result, countryId);
                    });
                break;
            }
        }
        if (found) {
            break;
        }
    }
}

void ServerController::connectToServerById(uint32_t id) {
    for (const auto &server : this->_allServers) {
        if (server.id == id) {
            auto countryId = server.countryId;
            AsyncProcess::execute(
                "nordvpn connect " + server.connectName, &this->_connectingPid,
                [this, countryId](const ProcessResult &result) {
                    this->_checkConnectResult(result, countryId);
                });
            break;
        }
    }
}

void ServerController::_checkConnectResult(const ProcessResult &result,
                                           int32_t countryId) {
    if (!result.success() && result.output.find("login") != std::string::npos) {
        // if the process returned an error code and the output contains the
        // word "login" this means, we tried to connect despite not being logged
        // in --> the the EnvController about it, that will propagate it to the
        // UI so that the login view will be displayed
        EnvController::getInstance().setLoggedIn(false);
    } else {
        // otherwise we assume a successful connection establishment and update
        // the recents list
        PreferencesRepository::addRecentCountryId(countryId);
        this->_recents = this->getRecentCountries();
        this->_notifySubscribersRecents();
    }
}

void ServerController::connectToSpecialtyGroup(Group g) {
    if (g != Group::Undefined) {
        AsyncProcess::execute("nordvpn connect " + group2connectName(g),
                              &this->_connectingPid);
    }
}

void ServerController::connectToCountryByIdAndGroup(uint32_t id, Group g) {
    std::string groupName = group2connectName(g);
    if (groupName.empty()) {
        this->connectToCountryById(id);
    } else {
        for (const auto &country : this->_allCountries) {
            if (country.id == id) {
                AsyncProcess::execute("nordvpn connect --group " + groupName +
                                          " " + country.connectName,
                                      &this->_connectingPid,
                                      [this, id](const ProcessResult &result) {
                                          this->_checkConnectResult(result, id);
                                      });
                break;
            }
        }
    }
}

void ServerController::cancelConnection() const {
    // try to kill the process that is responsible for establishing the
    // connection --> might not work
    AsyncProcess::kill(this->_connectingPid, true);
    // call disconnect as an additional measure since in most cases killing
    // the connecting process does not prevent nordvpn from finishing the
    // connection establishment, but calling disconnect while connection
    // ensures that nordvpn aborts its ongoing connection operation
    ServerController::disconnect();
}

void ServerController::disconnect() {
    AsyncProcess::execute("nordvpn disconnect");
}

void ServerController::_notifySubscribersRecents() {
    for (auto &subscriber : this->subscribers) {
        if (subscriber != nullptr) {
            subscriber->updateRecents(this->_recents);
        }
    }
}

void ServerController::_notifySubscribersCountryList() {
    for (auto &subscriber : this->subscribers) {
        if (subscriber != nullptr) {
            subscriber->updateCountryList(this->_allCountries);
        }
    }
}

void ServerController::_backgroundTaskServerList() {
    this->_allServers = ServerRepository::fetchServers();
}

void ServerController::_backgroundTaskCountryList(bool isSpecialTick) {
    // update the cache the first time (on app start) and then only every
    // 24h (it's an unlikely use case, that the list of countries changes)
    // See registerBackgroundTask(...) to change the isSpecialTick interval.
    this->getAllCountries(isSpecialTick);
    this->_notifySubscribersCountryList();
}

void ServerController::updateConnectionInfo(const ConnectionInfo &newInfo) {
    if (this->_quickConnecting &&
        newInfo.status == ConnectionStatus::Connected) {
        this->_quickConnecting = false;
        PreferencesRepository::addRecentCountryId(newInfo.countryId);
        this->_recents = this->getRecentCountries();
        this->_notifySubscribersRecents();
    }
}
