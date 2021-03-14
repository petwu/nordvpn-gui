#include "connectioncontroller.hpp"

#include <string>
#include <vector>

#include "common/io/asyncprocess.hpp"
#include "common/types/nullable.hpp"
#include "countrycontroller.hpp"
#include "data/models/country.hpp"
#include "data/models/location.hpp"
#include "data/models/server.hpp"
#include "envcontroller.hpp"
#include "logic/enums/connectionstatus.hpp"
#include "logic/nordvpn/statuscontroller.hpp"
#include "recentscontroller.hpp"
#include "servercontroller.hpp"

ConnectionController::ConnectionController() {
    // subscribe to connection status updates
    StatusController::getInstance().attach(this);
}

auto ConnectionController::getInstance() -> ConnectionController & {
    static ConnectionController instance;
    return instance;
}

void ConnectionController::quickConnect() {
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

void ConnectionController::connectToCountryById(uint32_t id) {
    Nullable<Country> c = CountryController::getInstance().getCountryById(id);
    if (c.isNotNull()) {
        AsyncProcess::execute("nordvpn connect " + c.value().connectName,
                              &this->_connectingPid,
                              [this, id](const ProcessResult &result) {
                                  _checkConnectResult(result, id);
                              });
    }
}

void ConnectionController::connectToCityById(uint32_t id) {
    Nullable<Country> country =
        CountryController::getInstance().getCountryByCityId(id);
    Nullable<Location> city = CountryController::getInstance().getCityById(id);
    if (country.isNotNull() && city.isNotNull()) {
        auto countryId = country.value().id;
        AsyncProcess::execute(
            "nordvpn connect " + country.value().connectName + " " +
                city.value().connectName,
            &this->_connectingPid,
            [this, countryId](const ProcessResult &result) { //
                _checkConnectResult(result, countryId);
            });
    }
}

void ConnectionController::connectToServerById(uint32_t id) {
    for (const auto &server : ServerController::getInstance().getAllServers()) {
        if (server.id == id) {
            auto countryId = server.countryId;
            AsyncProcess::execute(
                "nordvpn connect " + server.connectName, &this->_connectingPid,
                [this, countryId](const ProcessResult &result) {
                    _checkConnectResult(result, countryId);
                });
            break;
        }
    }
}

void ConnectionController::_checkConnectResult(const ProcessResult &result,
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
        RecentsController::getInstance().addTooRecentsList(countryId);
    }
}

void ConnectionController::connectToSpecialtyGroup(Group g) {
    if (g != Group::Undefined) {
        AsyncProcess::execute("nordvpn connect " + group2connectName(g),
                              &this->_connectingPid);
    }
}

void ConnectionController::connectToCountryByIdAndGroup(uint32_t id, Group g) {
    std::string groupName = group2connectName(g);
    if (groupName.empty()) {
        this->connectToCountryById(id);
    } else {
        Nullable<Country> c =
            CountryController::getInstance().getCountryById(id);
        if (c.isNotNull()) {
            AsyncProcess::execute("nordvpn connect --group " + groupName + " " +
                                      c.value().connectName,
                                  &this->_connectingPid,
                                  [this, id](const ProcessResult &result) {
                                      _checkConnectResult(result, id);
                                  });
        }
    }
}

void ConnectionController::cancelConnection() const {
    // try to kill the process that is responsible for establishing the
    // connection --> might not work
    AsyncProcess::kill(this->_connectingPid, true);
    // call disconnect as an additional measure since in most cases killing
    // the connecting process does not prevent nordvpn from finishing the
    // connection establishment, but calling disconnect while connection
    // ensures that nordvpn aborts its ongoing connection operation
    ConnectionController::disconnect();
}

void ConnectionController::disconnect() {
    AsyncProcess::execute("nordvpn disconnect");
}

void ConnectionController::updateConnectionInfo(const ConnectionInfo &newInfo) {
    if (this->_quickConnecting &&
        newInfo.status == ConnectionStatus::Connected) {
        this->_quickConnecting = false;
        RecentsController::getInstance().addTooRecentsList(newInfo.countryId);
    }
}
