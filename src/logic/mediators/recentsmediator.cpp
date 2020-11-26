#include "recentsmediator.h"

RecentsMediator::RecentsMediator() { //
    this->_serverController.attach(this);
}

QVariantList RecentsMediator::_getRecentCountries() {
    this->_recentCountries = this->_serverController.getRecentCountries();
    QVariantList recents;
    for (auto r : this->_recentCountries) {
        recents << QmlDataConverter::countryToQml(r);
    }
    return std::move(recents);
}

void RecentsMediator::removeFromRecentsList(quint32 countryId) {
    this->_serverController.removeFromRecentsList(countryId);
}

void RecentsMediator::updateRecents(const std::vector<Country> &newRecents) {
    this->_recentCountries = newRecents;
    this->recentCountriesChanged(this->_getRecentCountries());
}
