#include "recentsmediator.h"

#include <utility>

#include "logic/mediators/qmldataconverter.h"

RecentsMediator::RecentsMediator() { //
    this->_serverController.attach(this);
}

auto RecentsMediator::_getRecentCountries() -> QVariantList {
    this->_recentCountries = this->_serverController.getRecentCountries();
    QVariantList recents;
    for (const auto &r : this->_recentCountries) {
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
