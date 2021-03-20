#include "connectionmediator.hpp"

#include <thread>
#include <utility>

#include "config.hpp"
#include "logic/enums/connectionstatus.hpp"
#include "logic/mediators/qmldataconverter.hpp"
#include "logic/nordvpn/countrycontroller.hpp"

ConnectionMediator::ConnectionMediator() {
    this->_countryController.attach(this, true);
    this->_countryController.startBackgroundTasks();
    this->_statusController.attach(this);
    this->_statusController.startBackgroundTasks();
    this->_serverController.startBackgroundTasks();
}

void ConnectionMediator::updateConnectionInfo(const ConnectionInfo &newInfo) {
    int32_t connectingId = -1;
    int32_t connectedId = -1;
    int32_t connectedCityId = -1;
    bool disconnected = false;
    bool connecting = false;
    bool connected = false;
    switch (newInfo.status) {
    case ConnectionStatus::Disconnected:
        disconnected = true;
        break;
    case ConnectionStatus::Connecting:
        connecting = true;
        connectingId = newInfo.countryId;
        break;
    case ConnectionStatus::Connected:
        connected = true;
        connectedId = newInfo.countryId;
        connectedCityId = newInfo.cityId;
        break;
    }

    if (this->_areConnectionCommandsPaused) {
        // disable command-pausing when connection status changes
        if (this->_isConnected != connected ||
            this->_isConnecting != connecting ||
            this->_isDisconnected != disconnected) {
            this->_setAreConnectionCommandsPaused(false);
        }
    }
    if (connecting) {
        // pause command handling while connecting
        this->_setAreConnectionCommandsPaused(true);
    }

    if (this->_isConnected && disconnected) {
        // allow rating only when transitioning from connected to disconnected
        this->_setIsRatingPossible(true);
    } else if (this->_isDisconnected && !disconnected) {
        // disallow rating when not disconnected anymore
        this->_setIsRatingPossible(false);
    }

    this->_setIsDisconnected(disconnected);
    this->_setIsConnecting(connecting);
    this->_setIsConnected(connected);
    this->_setConnectingCountryId(connectingId);
    this->_setConnectedCountryId(connectedId);
    this->_setConnectedCityId(connectedCityId);
    this->_setConnectedServerNr(newInfo.serverNr);
    this->_setConnectedIP(newInfo.ip);
    this->_setReceivedBytes(newInfo.received);
    this->_setSentBytes(newInfo.sent);
    this->_setUptimeSeconds(newInfo.uptime);
    if (connected) {
        this->_setConnectedServerGroups(newInfo.groups);
    } else {
        this->_setConnectedServerGroups(std::vector<Group>{});
    }
    if (connecting) {
        this->_setConnectingServerGroups(newInfo.groups);
    } else {
        this->_setConnectingServerGroups(std::vector<Group>{});
    }
}

void ConnectionMediator::updateCountryList(
    const std::vector<Country> &countryList) {
    this->_countries = countryList;
    this->countryListChanged(this->_getCountryList());
}

auto ConnectionMediator::_getCountryList() -> QVariant {
    QVariantList list;
    for (const auto &country : this->_countries) {
        list << QmlDataConverter::countryToQml(country);
    }
    return list;
}

auto ConnectionMediator::_getAreConnectionCommandsPaused() const -> bool {
    return this->_areConnectionCommandsPaused;
}

void ConnectionMediator::_setAreConnectionCommandsPaused(bool value) {
    if (value != this->_areConnectionCommandsPaused) {
        this->_areConnectionCommandsPaused = value;
        this->areConnectionCommandsPausedChanged(value);
        // timeout to prevent blocking states
        if (value) {
            std::thread([this] {
                std::this_thread::sleep_for(
                    config::consts::CONNECTION_COMMANDS_PAUSE_PERIOD);
                this->_setAreConnectionCommandsPaused(false);
            }).detach();
        }
    }
}

auto ConnectionMediator::_getIsDisconnected() const -> bool {
    return this->_isDisconnected;
}

void ConnectionMediator::_setIsDisconnected(bool value) {
    if (value != this->_isDisconnected) {
        this->_isDisconnected = value;
        this->disconnectedChanged(value);
    }
}

auto ConnectionMediator::_getIsConnecting() const -> bool {
    return this->_isConnecting;
}

void ConnectionMediator::_setIsConnecting(bool value) {
    if (value != this->_isConnecting) {
        this->_isConnecting = value;
        this->connectingChanged(value);
    }
}

auto ConnectionMediator::_getIsConnected() const -> bool {
    return this->_isConnected;
}

void ConnectionMediator::_setIsConnected(bool value) {
    if (value != this->_isConnected) {
        this->_isConnected = value;
        this->connectedChanged(value);
    }
}

auto ConnectionMediator::_getIsRatingPossible() const -> bool {
    return this->_isRatingPossible;
}

void ConnectionMediator::_setIsRatingPossible(bool value) {
    if (value != this->_isRatingPossible) {
        this->_isRatingPossible = value;
        this->isRatingPossibleChanged(value);
        if (value) {
            // set to false (= hide rating widget) after a period of 1 min,
            // this should give everybody enough time to commit their rating
            std::thread([this] {
                std::this_thread::sleep_for(config::consts::RATING_PERIOD);
                if (this->_isRatingPossible) {
                    this->_setIsRatingPossible(false);
                }
            }).detach();
        }
    }
}

auto ConnectionMediator::_getConnectingCountryId() const -> qint32 {
    return this->_connectingCountryId;
}

void ConnectionMediator::_setConnectingCountryId(qint32 value) {
    if (value != this->_connectingCountryId) {
        this->_connectingCountryId = value;
        this->connectingCountryIdChanged(value);
    }
}

auto ConnectionMediator::_getConnectedCountryId() const -> qint32 {
    return this->_connectedCountryId;
}

void ConnectionMediator::_setConnectedCountryId(qint32 value) {
    if (value != this->_connectedCountryId) {
        this->_connectedCountryId = value;
        this->connectedCountryIdChanged(value);
    }
}

auto ConnectionMediator::_getConnectedCityId() const -> qint32 {
    return this->_connectedCityId;
}

void ConnectionMediator::_setConnectedCityId(qint32 value) {
    if (value != this->_connectedCityId) {
        this->_connectedCityId = value;
        this->connectedCityIdChanged(value);
    }
}

auto ConnectionMediator::_getConnectedServerNr() const -> qint32 {
    return this->_connectedServerNr;
}

void ConnectionMediator::_setConnectedServerNr(qint32 value) {
    if (value != this->_connectedServerNr) {
        this->_connectedServerNr = value;
        this->connectedServerNrChanged(value);
    }
}

auto ConnectionMediator::_getConnectedIP() -> QString {
    return QString(this->_connectedIP.c_str());
}

void ConnectionMediator::_setConnectedIP(const std::string &value) {
    if (value != this->_connectedIP) {
        this->_connectedIP = value;
        this->connectedIPChanged(QString(value.c_str()));
    }
}

auto ConnectionMediator::_getReceivedBytes() const -> qint64 {
    return this->_receivedBytes;
}

void ConnectionMediator::_setReceivedBytes(uint64_t value) {
    if (value != this->_receivedBytes) {
        this->_receivedBytes = value;
        this->receivedBytesChanged(value);
    }
}

auto ConnectionMediator::_getSentBytes() const -> qint64 {
    return this->_sentBytes;
}

void ConnectionMediator::_setSentBytes(uint64_t value) {
    if (value != this->_sentBytes) {
        this->_sentBytes = value;
        this->sentBytesChanged(value);
    }
}

auto ConnectionMediator::_getUptimeSeconds() const -> qint64 {
    return this->_uptimeSeconds;
}

void ConnectionMediator::_setUptimeSeconds(uint64_t value) {
    if (value != this->_uptimeSeconds) {
        this->_uptimeSeconds = value;
        this->uptimeSecondsChanged(value);
    }
}

auto ConnectionMediator::_getConnectedServerGroups() -> QVariantList {
    QVariantList serverGroups;
    for (auto group : this->_connectedServerGroups) {
        serverGroups << static_cast<int>(group);
    }
    return std::move(serverGroups);
}

void ConnectionMediator::_setConnectedServerGroups(std::vector<Group> groups) {
    this->_connectedServerGroups = std::move(groups);
    this->connectedServerGroupsChanged(this->_getConnectedServerGroups());
}

auto ConnectionMediator::_getConnectingServerGroups() -> QVariantList {
    QVariantList serverGroups;
    for (auto group : this->_connectingServerGroups) {
        serverGroups << static_cast<int>(group);
    }
    return std::move(serverGroups);
}

void ConnectionMediator::_setConnectingServerGroups(std::vector<Group> groups) {
    this->_connectingServerGroups = std::move(groups);
    this->connectingServerGroupsChanged(this->_getConnectingServerGroups());
}

auto ConnectionMediator::getServers(qint32 countryId, qint32 cityId)
    -> QVariantList {
    QVariantList servers;
    if (cityId < 0) {
        for (const auto &s :
             this->_serverController.getServersByCountry(countryId)) {
            servers << QmlDataConverter::serverToQml(s);
        }
    } else {
        for (const auto &s : this->_serverController.getServersByCity(cityId)) {
            servers << QmlDataConverter::serverToQml(s);
        }
    }
    return std::move(servers);
}

auto ConnectionMediator::getSpecialtyCountries(quint32 groupId)
    -> QVariantList {
    QVariantList countries;
    auto group = groupFromInt(groupId);
    if (group != Group::Undefined) {
        for (const auto &country :
             this->_countryController.getCountriesByGroup(group)) {
            countries << QmlDataConverter::countryToQml(country);
        }
    }
    return std::move(countries);
}

auto ConnectionMediator::getSpecialtyServers(quint32 groupId, qint32 countryId)
    -> QVariantList {
    QVariantList servers;
    auto group = groupFromInt(groupId);
    if (group != Group::Undefined) {
        for (const auto &server :
             this->_serverController.getServersByGroup(group, countryId)) {
            servers << QmlDataConverter::serverToQml(server);
        }
    }
    return std::move(servers);
}

void ConnectionMediator::quickConnect() {
    if (this->_areConnectionCommandsPaused) {
        return;
    }
    this->_connectionController.quickConnect();
    this->_setAreConnectionCommandsPaused(true);
}

void ConnectionMediator::connectToCountryById(quint32 id) {
    if (this->_areConnectionCommandsPaused) {
        return;
    }
    this->_connectionController.connectToCountryById(id);
    this->_setAreConnectionCommandsPaused(true);
}

void ConnectionMediator::connectToCityById(quint32 id) {
    if (this->_areConnectionCommandsPaused) {
        return;
    }
    this->_connectionController.connectToCityById(id);
    this->_setAreConnectionCommandsPaused(true);
}

void ConnectionMediator::connectToServerById(quint32 serverId) {
    this->_connectionController.connectToServerById(serverId);
}

void ConnectionMediator::connectToSpecialtyGroup(quint32 groupId) {
    Group group = groupFromInt(groupId);
    this->_connectionController.connectToSpecialtyGroup(group);
}

void ConnectionMediator::connectToCountryByIdAndGroup(quint32 id,
                                                      quint32 groupId) {
    Group group = groupFromInt(groupId);
    this->_connectionController.connectToCountryByIdAndGroup(id, group);
}

void ConnectionMediator::cancelConnection() {
    this->_connectionController.cancelConnection();
}

void ConnectionMediator::disconnect() {
    if (this->_areConnectionCommandsPaused) {
        return;
    }
    this->_connectionController.disconnect();
    this->_setAreConnectionCommandsPaused(true);
}

void ConnectionMediator::rate(quint8 rating) { //
    this->_statusController.rate(rating);
}
