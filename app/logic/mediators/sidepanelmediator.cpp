#include "sidepanelmediator.h"

#include "sidepanelmediator.h"

QVariantList SidePanelMediator::_getRecentCountries() {
    if (this->_recentCountries.size() == 0) {
        this->_recentCountries = this->_serverController.getRecentCountries();
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
