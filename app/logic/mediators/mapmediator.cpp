#include "mapmediator.h"

MapMediator::MapMediator() {
    this->_statusController.attach(this);
    this->_statusController.startBackgroundTask();
    this->_countries = this->_serverController.getAllCountries();
}

void MapMediator::update(const ConnectionInfo &newInfo) {
    int32_t connectingId, connectedId;
    switch (newInfo.status) {
    case ConnectionStatus::Disconnected:
        connectingId = -1;
        connectedId = -1;
        break;
    case ConnectionStatus::Connecting:
        connectingId = this->_getCountryId(newInfo.country);
        connectedId = -1;
        break;
    case ConnectionStatus::Connected:
        connectingId = -1;
        connectedId = this->_getCountryId(newInfo.country);
        break;
    }
    this->_connectingCountryId = connectingId;
    this->connectingIdChanged(connectingId);
    this->_connectedCountryId = connectedId;
    this->connectedIdChanged(connectedId);
}

QVariant MapMediator::getAllCountries() {
    return QmlDataConverter::jsonToQml(this->_countries);
}

qint32 MapMediator::_getConnectingCountryId() {
    return this->_connectingCountryId;
}

qint32 MapMediator::_getConnectedCountryId() {
    return this->_connectedCountryId;
}

int32_t MapMediator::_getCountryId(std::string name) {
    json countries = this->_countries;
    for (auto i = countries.begin(); i != countries.end(); ++i) {
        json country = i.value();
        if (country.is_object() && country["id"].is_number_integer() &&
            (country["statusName"].is_string() ||
             country["connectName"].is_string())) {
            std::string sName = util::string::toLower(
                country["statusName"].is_string() ? country["statusName"] : "");
            std::string cName = util::string::toLower(
                country["connectName"].is_string() ? country["connectName"]
                                                   : "");
            std::string n1 = util::string::toLower(name);
            std::string n2 = util::string::replaceAll(n1, "_", " ");
            if (sName == n1 || sName == n2 || cName == n1 || cName == n2) {
                return country["id"];
            }
        }
    }
    return -1;
}
