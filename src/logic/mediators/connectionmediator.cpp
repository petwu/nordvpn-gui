#include "connectionmediator.h"

ConnectionMediator::ConnectionMediator() {
    this->_countries = this->_serverController.getAllCountries();
    this->_statusController.attach(this);
    this->_statusController.startBackgroundTask();
    this->_serverController.attach(this);
    this->_serverController.startBackgroundTask();
}

void ConnectionMediator::updateConnectionInfo(const ConnectionInfo &newInfo) {
    int32_t connectingId = -1, connectedId = -1, connectedCityId = -1;
    bool disconnected = false, connecting = false, connected = false;
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

    if (this->_isConnected && disconnected)
        // allow rating only when transitioning from connected to disconnected
        this->_setIsRatingPossible(true);
    else if (this->_isDisconnected && !disconnected)
        // disallow rating when not disconnected anymore
        this->_setIsRatingPossible(false);

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
    if (connected)
        this->_setConnectedServerGroups(newInfo.groups);
    else
        this->_setConnectedServerGroups(std::vector<Group>{});
    if (connecting)
        this->_setConnectingServerGroups(newInfo.groups);
    else
        this->_setConnectingServerGroups(std::vector<Group>{});
}

void ConnectionMediator::updateCountryList(
    const std::vector<Country> &countryList) {
    this->_countries = countryList;
    this->countryListChanged(this->_getCountryList());
}

QVariant ConnectionMediator::_getCountryList() {
    QVariantList list;
    for (auto country : this->_countries) {
        list << QmlDataConverter::countryToQml(country);
    }
    return list;
}

bool ConnectionMediator::_getAreConnectionCommandsPaused() {
    return this->_areConnectionCommandsPaused;
}

void ConnectionMediator::_setAreConnectionCommandsPaused(bool value) {
    if (value != this->_areConnectionCommandsPaused) {
        this->_areConnectionCommandsPaused = value;
        this->areConnectionCommandsPausedChanged(value);
        // timeout to prevent blocking states
        if (value == true)
            std::thread([this] {
                std::this_thread::sleep_for(
                    config::consts::CONNECTION_COMMANDS_PAUSE_PERIOD);
                this->_setAreConnectionCommandsPaused(false);
            }).detach();
    }
}

bool ConnectionMediator::_getIsDisconnected() { return this->_isDisconnected; }

void ConnectionMediator::_setIsDisconnected(bool value) {
    if (value != this->_isDisconnected) {
        this->_isDisconnected = value;
        this->disconnectedChanged(value);
    }
}

bool ConnectionMediator::_getIsConnecting() { return this->_isConnecting; }

void ConnectionMediator::_setIsConnecting(bool value) {
    if (value != this->_isConnecting) {
        this->_isConnecting = value;
        this->connectingChanged(value);
    }
}

bool ConnectionMediator::_getIsConnected() { return this->_isConnected; }

void ConnectionMediator::_setIsConnected(bool value) {
    if (value != this->_isConnected) {
        this->_isConnected = value;
        this->connectedChanged(value);
    }
}

bool ConnectionMediator::_getIsRatingPossible() {
    return this->_isRatingPossible;
}

void ConnectionMediator::_setIsRatingPossible(bool value) {
    if (value != this->_isRatingPossible) {
        this->_isRatingPossible = value;
        this->isRatingPossibleChanged(value);
        if (value == true) {
            // set to false (= hide rating widget) after a period of 1 min,
            // this should give everybody enough time to commit their rating
            std::thread([this] {
                std::this_thread::sleep_for(config::consts::RATING_PERIOD);
                if (this->_isRatingPossible)
                    this->_setIsRatingPossible(false);
            }).detach();
        }
    }
}

qint32 ConnectionMediator::_getConnectingCountryId() {
    return this->_connectingCountryId;
}

void ConnectionMediator::_setConnectingCountryId(qint32 value) {
    if (value != this->_connectingCountryId) {
        this->_connectingCountryId = value;
        this->connectingCountryIdChanged(value);
    }
}

qint32 ConnectionMediator::_getConnectedCountryId() {
    return this->_connectedCountryId;
}

void ConnectionMediator::_setConnectedCountryId(qint32 value) {
    if (value != this->_connectedCountryId) {
        this->_connectedCountryId = value;
        this->connectedCountryIdChanged(value);
    }
}

qint32 ConnectionMediator::_getConnectedCityId() {
    return this->_connectedCityId;
}

void ConnectionMediator::_setConnectedCityId(qint32 value) {
    if (value != this->_connectedCityId) {
        this->_connectedCityId = value;
        this->connectedCityIdChanged(value);
    }
}

qint32 ConnectionMediator::_getConnectedServerNr() {
    return this->_connectedServerNr;
}

void ConnectionMediator::_setConnectedServerNr(qint32 value) {
    if (value != this->_connectedServerNr) {
        this->_connectedServerNr = value;
        this->connectedServerNrChanged(value);
    }
}

QString ConnectionMediator::_getConnectedIP() {
    return QString(this->_connectedIP.c_str());
}

void ConnectionMediator::_setConnectedIP(std::string value) {
    if (value != this->_connectedIP) {
        this->_connectedIP = value;
        this->connectedIPChanged(QString(value.c_str()));
    }
}

qint64 ConnectionMediator::_getReceivedBytes() { return this->_receivedBytes; }

void ConnectionMediator::_setReceivedBytes(uint64_t value) {
    if (value != this->_receivedBytes) {
        this->_receivedBytes = value;
        this->receivedBytesChanged(value);
    }
}

qint64 ConnectionMediator::_getSentBytes() { return this->_sentBytes; }

void ConnectionMediator::_setSentBytes(uint64_t value) {
    if (value != this->_sentBytes) {
        this->_sentBytes = value;
        this->sentBytesChanged(value);
    }
}

QVariantList ConnectionMediator::_getConnectedServerGroups() {
    QVariantList serverGroups;
    for (auto group : this->_connectedServerGroups)
        serverGroups << static_cast<int>(group);
    return std::move(serverGroups);
}

void ConnectionMediator::_setConnectedServerGroups(std::vector<Group> groups) {
    this->_connectedServerGroups = groups;
    this->connectedServerGroupsChanged(this->_getConnectedServerGroups());
}

QVariantList ConnectionMediator::_getConnectingServerGroups() {
    QVariantList serverGroups;
    for (auto group : this->_connectingServerGroups)
        serverGroups << static_cast<int>(group);
    return std::move(serverGroups);
}

void ConnectionMediator::_setConnectingServerGroups(std::vector<Group> groups) {
    this->_connectingServerGroups = groups;
    this->connectingServerGroupsChanged(this->_getConnectingServerGroups());
}

QVariantList ConnectionMediator::getServers(qint32 countryId, qint32 cityId) {
    QVariantList servers;
    if (cityId < 0) {
        for (auto s : this->_serverController.getServersByCountry(countryId))
            servers << QmlDataConverter::serverToQml(s);
    } else {
        for (auto s : this->_serverController.getServersByCity(cityId))
            servers << QmlDataConverter::serverToQml(s);
    }
    return std::move(servers);
}

QVariantList ConnectionMediator::getSpecialtyCountries(quint32 groupId) {
    QVariantList countries;
    auto group = groupFromInt(groupId);
    if (group != Group::Undefined)
        for (auto country : this->_serverController.getCountriesByGroup(group))
            countries << QmlDataConverter::countryToQml(country);
    return std::move(countries);
}

QVariantList ConnectionMediator::getSpecialtyServers(quint32 groupId,
                                                     qint32 countryId) {
    QVariantList servers;
    auto group = groupFromInt(groupId);
    if (group != Group::Undefined)
        for (auto server :
             this->_serverController.getServersByGroup(group, countryId))
            servers << QmlDataConverter::serverToQml(server);
    return std::move(servers);
}

void ConnectionMediator::quickConnect() {
    if (this->_areConnectionCommandsPaused)
        return;
    this->_serverController.quickConnect();
    this->_setAreConnectionCommandsPaused(true);
}

void ConnectionMediator::connectToCountryById(quint32 id) {
    if (this->_areConnectionCommandsPaused)
        return;
    this->_serverController.connectToCountryById(id);
    this->_setAreConnectionCommandsPaused(true);
}

void ConnectionMediator::connectToCityById(quint32 id) {
    if (this->_areConnectionCommandsPaused)
        return;
    this->_serverController.connectToCityById(id);
    this->_setAreConnectionCommandsPaused(true);
}

void ConnectionMediator::connectToServerById(quint32 serverId) {
    this->_serverController.connectToServerById(serverId);
}

void ConnectionMediator::connectToSpecialtyGroup(quint32 groupId) {
    Group group = groupFromInt(groupId);
    this->_serverController.connectToSpecialtyGroup(group);
}

void ConnectionMediator::connectToCountryByIdAndGroup(quint32 id,
                                                      quint32 groupId) {
    Group group = groupFromInt(groupId);
    this->_serverController.connectToCountryByIdAndGroup(id, group);
}

void ConnectionMediator::cancelConnection() {
    this->_serverController.cancelConnection();
}

void ConnectionMediator::disconnect() {
    if (this->_areConnectionCommandsPaused)
        return;
    this->_serverController.disconnect();
    this->_setAreConnectionCommandsPaused(true);
}

void ConnectionMediator::rate(quint8 rating) {
    this->_statusController.rate(rating);
}
