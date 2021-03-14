#include "servercontroller.hpp"

#include <thread>
#include <utility>

#include "common/types/nullable.hpp"
#include "config.hpp"
#include "data/enums/securityprotocol.hpp"
#include "data/repositories/serverrepository.hpp"
#include "logic/models/nordvpnsettings.hpp"
#include "logic/nordvpn/preferencescontroller.hpp"

ServerController::ServerController() {
    // register my background tasks
    this->registerBackgroundTask([this] { _backgroundTask(); },
                                 config::consts::SERVER_LIST_UPDATE_INTERVAL);
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

auto ServerController::getAllServers() -> std::vector<Server> {
    return this->_filterServerList(-1, -1);
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

auto ServerController::getServersByCountry(int32_t countryId)
    -> std::vector<Server> {
    return this->_filterServerList(countryId, -1);
}

auto ServerController::getServersByCity(int32_t cityId) -> std::vector<Server> {
    return this->_filterServerList(-1, cityId);
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

void ServerController::_backgroundTask() {
    this->_allServers = ServerRepository::fetchServers();
}
