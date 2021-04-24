#include "connectioncontroller.hpp"

#include <string>
#include <utility>
#include <vector>

#include "nordvpn/enums/connectionstatus.hpp"
#include "nordvpn/io/asyncprocess.hpp"
#include "nordvpn/models/country.hpp"
#include "nordvpn/models/location.hpp"
#include "nordvpn/models/nullable.hpp"
#include "nordvpn/models/server.hpp"

ConnectionController::ConnectionController(
    std::shared_ptr<ICountryController> countryController,
    std::shared_ptr<IEnvController> envController,
    std::shared_ptr<IRecentsController> recentsController,
    std::shared_ptr<IServerController> serverController,
    std::shared_ptr<IStatusController> statusController)
    : _countryController(std::move(countryController)),
      _envController(std::move(envController)),
      _recentsController(std::move(recentsController)),
      _serverController(std::move(serverController)),
      _statusController(std::move(statusController)) {
    // subscribe to connection status updates
    this->_statusController->attach(this);
}

void ConnectionController::quickConnect() {
    this->_quickConnecting = true;
    AsyncProcess::execute("nordvpn connect", &this->_connectingPid,
                          [this](const ProcessResult &result) {
                              if (!result.success() &&
                                  result.output.find("login") !=
                                      std::string::npos) {
                                  this->_quickConnecting = false;
                                  this->_envController->setLoggedIn(false);
                              }
                          });
}

void ConnectionController::connectToCountryById(uint32_t id) {
    Nullable<Country> c = this->_countryController->getCountryById(id);
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
        this->_countryController->getCountryByCityId(id);
    Nullable<Location> city = this->_countryController->getCityById(id);
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
    for (const auto &server : this->_serverController->getAllServers()) {
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
        this->_envController->setLoggedIn(false);
    } else {
        // otherwise we assume a successful connection establishment and update
        // the recents list
        this->_recentsController->addTooRecentsList(countryId);
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
        Nullable<Country> c = this->_countryController->getCountryById(id);
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

void ConnectionController::cancelConnection() {
    // try to kill the process that is responsible for establishing the
    // connection --> might not work
    AsyncProcess::kill(this->_connectingPid, true);
    // call disconnect as an additional measure since in most cases killing
    // the connecting process does not prevent nordvpn from finishing the
    // connection establishment, but calling disconnect while connection
    // ensures that nordvpn aborts its ongoing connection operation
    this->disconnect();
}

void ConnectionController::disconnect() {
    AsyncProcess::execute("nordvpn disconnect");
}

void ConnectionController::updateConnectionInfo(const ConnectionInfo &newInfo) {
    if (this->_quickConnecting &&
        newInfo.status == ConnectionStatus::Connected) {
        this->_quickConnecting = false;
        this->_recentsController->addTooRecentsList(newInfo.countryId);
    }
}
