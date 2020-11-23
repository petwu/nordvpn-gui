#include "mediator.h"

Mediator::Mediator() {
    this->_countries = this->_serverController.getAllCountries();
    this->_statusController.attach(this);
    this->_statusController.startBackgroundTask();
    this->_serverController.attach(this);
    this->_serverController.startBackgroundTask();
}

void Mediator::updateConnectionInfo(const ConnectionInfo &newInfo) {
    int32_t connectingId = -1, connectedId = -1;
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
    this->_setConnectedServerNr(newInfo.serverNr);
    this->_setConnectedIP(newInfo.ip);
    if (connected)
        this->_setConnectedServerGroups(newInfo.groups);
    else
        this->_setConnectedServerGroups(std::vector<Group>{});
    if (connecting)
        this->_setConnectingServerGroups(newInfo.groups);
    else
        this->_setConnectingServerGroups(std::vector<Group>{});
}

void Mediator::updateCountryList(const std::vector<Country> &countryList) {
    this->_countries = countryList;
    this->countryListChanged(this->_getCountryList());
}

QVariant Mediator::_getCountryList() {
    QVariantList list;
    for (auto country : this->_countries) {
        list << QmlDataConverter::countryToQml(country);
    }
    return list;
}

bool Mediator::_getAreConnectionCommandsPaused() {
    return this->_areConnectionCommandsPaused;
}

void Mediator::_setAreConnectionCommandsPaused(bool value) {
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

bool Mediator::_getIsDisconnected() { return this->_isDisconnected; }

void Mediator::_setIsDisconnected(bool value) {
    if (value != this->_isDisconnected) {
        this->_isDisconnected = value;
        this->disconnectedChanged(value);
    }
}

bool Mediator::_getIsConnecting() { return this->_isConnecting; }

void Mediator::_setIsConnecting(bool value) {
    if (value != this->_isConnecting) {
        this->_isConnecting = value;
        this->connectingChanged(value);
    }
}

bool Mediator::_getIsConnected() { return this->_isConnected; }

void Mediator::_setIsConnected(bool value) {
    if (value != this->_isConnected) {
        this->_isConnected = value;
        this->connectedChanged(value);
    }
}

bool Mediator::_getIsRatingPossible() { return this->_isRatingPossbile; }

void Mediator::_setIsRatingPossible(bool value) {
    if (value != this->_isRatingPossbile) {
        this->_isRatingPossbile = value;
        this->isRatingPossibleChanged(value);
        if (value == true) {
            // set to false (= hide rating widget) after a period of 1 min,
            // this should give everybody enough time to commit their rating
            std::thread([this] {
                std::this_thread::sleep_for(config::consts::RATING_PERIOD);
                if (this->_isRatingPossbile)
                    this->_setIsRatingPossible(false);
            }).detach();
        }
    }
}

qint32 Mediator::_getConnectingCountryId() {
    return this->_connectingCountryId;
}

void Mediator::_setConnectingCountryId(qint32 value) {
    if (value != this->_connectingCountryId) {
        this->_connectingCountryId = value;
        this->connectingIdChanged(value);
    }
}

qint32 Mediator::_getConnectedCountryId() { return this->_connectedCountryId; }

void Mediator::_setConnectedCountryId(qint32 value) {
    if (value != this->_connectedCountryId) {
        this->_connectedCountryId = value;
        this->connectedIdChanged(value);
    }
}

qint32 Mediator::_getConnectedServerNr() { return this->_connectedServerNr; }

void Mediator::_setConnectedServerNr(qint32 value) {
    if (value != this->_connectedServerNr) {
        this->_connectedServerNr = value;
        this->connectedServerNrChanged(value);
    }
}

QString Mediator::_getConnectedIP() {
    return QString(this->_connectedIP.c_str());
}

void Mediator::_setConnectedIP(std::string value) {
    if (value != this->_connectedIP) {
        this->_connectedIP = value;
        this->connectedIPChanged(QString(value.c_str()));
    }
}

QVariantList Mediator::_getRecentCountries() {
    if (this->_recentCountries.size() == 0) {
        this->_recentCountries = this->_serverController.getRecentCountries();
    }
    QVariantList recents;
    for (auto r : this->_recentCountries) {
        recents << QmlDataConverter::countryToQml(r);
    }
    return std::move(recents);
}

void Mediator::updateRecents(const std::vector<Country> &newRecents) {
    this->_recentCountries = newRecents;
    this->recentCountriesChanged(this->_getRecentCountries());
}

void Mediator::removeFromRecentsList(quint32 countryId) {
    this->_serverController.removeFromRecentsList(countryId);
}

QVariantList Mediator::_getConnectedServerGroups() {
    QVariantList serverGroups;
    for (auto group : this->_connectedServerGroups)
        serverGroups << static_cast<int>(group);
    return std::move(serverGroups);
}

void Mediator::_setConnectedServerGroups(std::vector<Group> groups) {
    this->_connectedServerGroups = groups;
    this->connectedServerGroupsChanged(this->_getConnectedServerGroups());
}

QVariantList Mediator::_getConnectingServerGroups() {
    QVariantList serverGroups;
    for (auto group : this->_connectingServerGroups)
        serverGroups << static_cast<int>(group);
    return std::move(serverGroups);
}

void Mediator::_setConnectingServerGroups(std::vector<Group> groups) {
    this->_connectingServerGroups = groups;
    this->connectingServerGroupsChanged(this->_getConnectingServerGroups());
}

QVariantList Mediator::getServers(qint32 countryId, qint32 cityId) {
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

QVariantList Mediator::getSpecialtyCountries(quint32 groupId) {
    QVariantList countries;
    auto group = groupFromInt(groupId);
    if (group != Group::Undefined)
        for (auto country : this->_serverController.getCountriesByGroup(group))
            countries << QmlDataConverter::countryToQml(country);
    return std::move(countries);
}

QVariantList Mediator::getSpecialtyServers(quint32 groupId, qint32 countryId) {
    QVariantList servers;
    auto group = groupFromInt(groupId);
    if (group != Group::Undefined)
        for (auto server :
             this->_serverController.getServersByGroup(group, countryId))
            servers << QmlDataConverter::serverToQml(server);
    return std::move(servers);
}

void Mediator::quickConnect() {
    if (this->_areConnectionCommandsPaused)
        return;
    this->_serverController.quickConnect();
    this->_setAreConnectionCommandsPaused(true);
}

void Mediator::connectToCountryById(quint32 id) {
    if (this->_areConnectionCommandsPaused)
        return;
    this->_serverController.connectToCountryById(id);
    this->_setAreConnectionCommandsPaused(true);
}

void Mediator::connectToServerById(quint32 serverId) {
    this->_serverController.connectToServerById(serverId);
}

void Mediator::connectToSpecialtyGroup(quint32 groupId) {
    Group group = groupFromInt(groupId);
    this->_serverController.connectToSpecialtyGroup(group);
}

void Mediator::cancelConnection() {
    this->_serverController.cancelConnection();
}

void Mediator::disconnect() {
    if (this->_areConnectionCommandsPaused)
        return;
    this->_serverController.disconnect();
    this->_setAreConnectionCommandsPaused(true);
}

void Mediator::rate(quint8 rating) { this->_statusController.rate(rating); }
