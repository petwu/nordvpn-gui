#include "mapmediator.h"

MapMediator::MapMediator() {
    this->_statusController.attach(this);
    this->_statusController.startBackgroundTask();
    this->_countries = this->_serverController.getAllCountries();
}

void MapMediator::update(const ConnectionInfo &newInfo) {
    std::cout << newInfo.toString() << std::endl;
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
    if (this->_connected && disconnected) {
        this->_isRatingPossbile = true;
        this->isRatingPossibleChanged(true);
    } else if (this->_disconnected && !disconnected) {
        this->_isRatingPossbile = false;
        this->isRatingPossibleChanged(false);
    }
    if (disconnected != this->_disconnected) {
        this->_disconnected = disconnected;
        this->disconnectedChanged(disconnected);
    }
    if (connecting != this->_connecting) {
        this->_connecting = connecting;
        this->connectingChanged(connecting);
    }
    if (connected != this->_connected) {
        this->_connected = connected;
        this->connectedChanged(connected);
    }
    if (connectingId != this->_connectingCountryId) {
        this->_connectingCountryId = connectingId;
        this->connectingIdChanged(connectingId);
    }
    if (connectedId != this->_connectedCountryId) {
        this->_connectedCountryId = connectedId;
        this->connectedIdChanged(connectedId);
    }
    if (newInfo.serverId != this->_connectedServerId) {
        this->_connectedServerId = newInfo.serverId;
        this->connectedServerIdChanged(newInfo.serverId);
    }
    if (newInfo.ip != this->_connectedIP) {
        this->_connectedIP = newInfo.ip;
        this->connectedIPChanged(QString(newInfo.ip.c_str()));
    }
}

QVariant MapMediator::_getCountryList() {
    return QmlDataConverter::jsonToQml(this->_countries);
}

bool MapMediator::_isDisconnected() { return this->_disconnected; }

bool MapMediator::_isConnecting() { return this->_connecting; }

bool MapMediator::_isConnected() { return this->_connected; }

bool MapMediator::_getIsRatingPossible() { return this->_isRatingPossbile; }

qint32 MapMediator::_getConnectingCountryId() {
    return this->_connectingCountryId;
}

qint32 MapMediator::_getConnectedCountryId() {
    return this->_connectedCountryId;
}

qint32 MapMediator::_getConnectedServerId() { return this->_connectedServerId; }

QString MapMediator::_getConnectedIP() {
    return QString(this->_connectedIP.c_str());
}

void MapMediator::quickConnect() { this->_serverController.quickConnect(); }

void MapMediator::connectToCountryById(quint32 id) {
    this->_serverController.connectToCountryById(id);
}

void MapMediator::disconnect() { this->_serverController.disconnect(); }

void MapMediator::rate(quint8 rating) { this->_statusController.rate(rating); }
