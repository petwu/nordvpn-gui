#include "mapmediator.h"

MapMediator::MapMediator() {
    this->_statusController.attach(this);
    this->_statusController.startBackgroundTask();
    this->_countries = this->_serverController.getAllCountries();
}

void MapMediator::update(const ConnectionInfo &newInfo) {
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
    } else if (connecting) {
        // pause command handling while connecting
        this->_setAreConnectionCommandsPaused(true);
    }

    if (this->_isConnected && disconnected) {
        // allow rating only when transitioning from connected to disconnected
        this->_isRatingPossbile = true;
        this->isRatingPossibleChanged(true);
    } else if (this->_isDisconnected && !disconnected) {
        // disallow rating when not disconnected anymore
        this->_isRatingPossbile = false;
        this->isRatingPossibleChanged(false);
    }

    this->_setIsDisconnected(disconnected);
    this->_setIsConnecting(connecting);
    this->_setIsConnected(connected);
    this->_setConnectingCountryId(connectingId);
    this->_setConnectedCountryId(connectedId);
    this->_setConnectedServerId(newInfo.serverId);
    this->_setConnectedIP(newInfo.ip);
}

QVariant MapMediator::_getCountryList() {
    QVariantList list;
    for (auto country : this->_countries) {
        list << QmlDataConverter::countryToQml(country);
    }
    return list;
}

bool MapMediator::_getAreConnectionCommandsPaused() {
    return this->_areConnectionCommandsPaused;
}

void MapMediator::_setAreConnectionCommandsPaused(bool value) {
    if (value != this->_areConnectionCommandsPaused) {
        this->_areConnectionCommandsPaused = value;
        this->areConnectionCommandsPausedChanged(value);
    }
}

bool MapMediator::_getIsDisconnected() { return this->_isDisconnected; }

void MapMediator::_setIsDisconnected(bool value) {
    if (value != this->_isDisconnected) {
        this->_isDisconnected = value;
        this->disconnectedChanged(value);
    }
}

bool MapMediator::_getIsConnecting() { return this->_isConnecting; }

void MapMediator::_setIsConnecting(bool value) {
    if (value != this->_isConnecting) {
        this->_isConnecting = value;
        this->connectingChanged(value);
    }
}

bool MapMediator::_getIsConnected() { return this->_isConnected; }

void MapMediator::_setIsConnected(bool value) {
    if (value != this->_isConnected) {
        this->_isConnected = value;
        this->connectedChanged(value);
    }
}

bool MapMediator::_getIsRatingPossible() { return this->_isRatingPossbile; }

void MapMediator::_setIsRatingPossible(bool value) {
    if (value != this->_isRatingPossbile) {
        this->_isRatingPossbile = value;
        this->isRatingPossibleChanged(value);
    }
}

qint32 MapMediator::_getConnectingCountryId() {
    return this->_connectingCountryId;
}

void MapMediator::_setConnectingCountryId(qint32 value) {
    if (value != this->_connectingCountryId) {
        this->_connectingCountryId = value;
        this->connectingIdChanged(value);
    }
}

qint32 MapMediator::_getConnectedCountryId() {
    return this->_connectedCountryId;
}

void MapMediator::_setConnectedCountryId(qint32 value) {
    if (value != this->_connectedCountryId) {
        this->_connectedCountryId = value;
        this->connectedIdChanged(value);
    }
}

qint32 MapMediator::_getConnectedServerId() { return this->_connectedServerId; }

void MapMediator::_setConnectedServerId(qint32 value) {
    if (value != this->_connectedServerId) {
        this->_connectedServerId = value;
        this->connectedServerIdChanged(value);
    }
}

QString MapMediator::_getConnectedIP() {
    return QString(this->_connectedIP.c_str());
}

void MapMediator::_setConnectedIP(std::string value) {
    if (value != this->_connectedIP) {
        this->_connectedIP = value;
        this->connectedIPChanged(QString(value.c_str()));
    }
}

void MapMediator::quickConnect() {
    if (this->_areConnectionCommandsPaused)
        return;
    this->_serverController.quickConnect();
    this->_setAreConnectionCommandsPaused(true);
}

void MapMediator::connectToCountryById(quint32 id) {
    if (this->_areConnectionCommandsPaused)
        return;
    this->_serverController.connectToCountryById(id);
    this->_setAreConnectionCommandsPaused(true);
}

void MapMediator::disconnect() {
    if (this->_areConnectionCommandsPaused)
        return;
    this->_serverController.disconnect();
    this->_setAreConnectionCommandsPaused(true);
}

void MapMediator::rate(quint8 rating) { this->_statusController.rate(rating); }
