#include "recentsmediator.h"

#include <utility>

#include "logic/mediators/qmldataconverter.h"

RecentsMediator::RecentsMediator() { //
    this->_recentsController.attach(this);
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
