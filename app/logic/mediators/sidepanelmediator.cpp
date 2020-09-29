#include "sidepanelmediator.h"

#include "sidepanelmediator.h"

SidePanelMediator::SidePanelMediator(std::shared_ptr<ServerController> sc)
    : _serverController(sc) {
    this->_serverController->attach(this);
}

QVariantList SidePanelMediator::_getRecentCountries() {
    if (this->_recentCountries.size() == 0) {
        this->_recentCountries = this->_serverController->getRecentCountries();
    }
    QVariantList recents;
    for (auto r : this->_recentCountries) {
        recents << QmlDataConverter::countryToQml(r);
    }
    return std::move(recents);
}

void SidePanelMediator::updateRecents(const std::vector<Country> &newRecents) {
    this->_recentCountries = newRecents;
    this->recentCountriesChanged(this->_getRecentCountries());
}

void SidePanelMediator::removeFromRecentsList(quint32 countryId) {
    this->_serverController->removeFromRecentsList(countryId);
}
