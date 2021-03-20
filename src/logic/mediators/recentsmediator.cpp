#include "recentsmediator.hpp"

#include <utility>

#include "logic/mediators/qmldataconverter.hpp"

RecentsMediator::RecentsMediator() { //
    this->_recentsController.attach(this, true);
}

auto RecentsMediator::_getRecentCountries() -> QVariantList {
    QVariantList recents;
    for (const auto &r : this->_recentCountries) {
        recents << QmlDataConverter::countryToQml(r);
    }
    return std::move(recents);
}

void RecentsMediator::removeFromRecentsList(quint32 countryId) {
    this->_recentsController.removeFromRecentsList(countryId);
}

void RecentsMediator::updateRecents(const std::vector<Country> &newRecents) {
    this->_recentCountries = newRecents;
    this->recentCountriesChanged(this->_getRecentCountries());
}
