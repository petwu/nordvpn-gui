#include "countrycontroller.hpp"

#include <string>
#include <utility>

#include "nordvpn/config.hpp"
#include "nordvpn/io/process.hpp"
#include "nordvpn/io/processresult.hpp"
#include "nordvpn/models/connectable.hpp"
#include "nordvpn/models/server.hpp"
#include "util/strings.hpp"

CountryController::CountryController(
    std::shared_ptr<IServerController> serverController,
    std::shared_ptr<IServerRepository> serverRepository)
    : _serverController(std::move(serverController)),
      _serverRepository(std::move(serverRepository)) {
    this->registerBackgroundTask([this](bool tick) { _backgroundTask(tick); },
                                 config::consts::COUNTRY_LIST_UPDATE_INTERVAL,
                                 config::consts::COUNTRY_LIST_REFETCH_INTERVAL);
    this->_getAllCountries(false);
}

auto CountryController::getAllCountries(bool updateCache)
    -> std::vector<Country> {
    return this->_getAllCountries(updateCache);
}

auto CountryController::_getAllCountries(bool updateCache)
    -> std::vector<Country> {
    if (this->_allCountries.empty() || updateCache) {
        auto cmdResult = Process::execute("nordvpn countries");
        auto cliCountries = util::string::split(cmdResult.output, ", ");
        std::vector<Country> all;
        if (updateCache) {
            all = this->_serverRepository->fetchCountries();
        } else {
            all = this->_serverRepository->fetchCountriesFromCache();
        }
        std::vector<Country> availableCountries;
        // filter the vector to only contains countries that were returned
        // by the CLI command
        for (const std::string &c : cliCountries) {
            for (const auto &e : all) {
                if (c == e.connectName) {
                    availableCountries.push_back(e);
                }
            }
        }
        // filter the countries to only contain cities that were returned by the
        // CLI command
        for (auto &availableCountry : availableCountries) {
            cmdResult = Process::execute("nordvpn cities " +
                                         availableCountry.connectName);
            auto cliCities = util::string::split(cmdResult.output, ", ");
            std::vector<Location> availableCities;
            for (const auto &city : availableCountry.cities) {
                for (const auto &cliCity : cliCities) {
                    if (Connectable::fuzzyMatchNames(city.name, cliCity)) {
                        availableCities.push_back(city);
                    }
                }
            }
            availableCountry.cities = availableCities;
        }
        this->_allCountries = availableCountries;
    }
    return this->_allCountries;
}

auto CountryController::getCountryById(uint32_t id) -> Nullable<Country> {
    for (const auto &country : this->_allCountries) {
        if (country.id == id) {
            return country;
        }
    }
    return Nullable<Country>();
}

auto CountryController::getCountryByCityId(uint32_t id) -> Nullable<Country> {
    for (const auto &country : this->_allCountries) {
        for (const auto &city : country.cities) {
            if (city.id == id) {
                return country;
            }
        }
    }
    return Nullable<Country>();
}

auto CountryController::getCityById(uint32_t id) -> Nullable<Location> {
    for (const auto &country : this->_allCountries) {
        for (const auto &city : country.cities) {
            if (city.id == id) {
                return city;
            }
        }
    }
    return Nullable<Location>();
}

auto CountryController::getCountriesByGroup(Group g) -> std::vector<Country> {
    std::vector<Country> countries;
    for (const auto &server : this->_serverController->getAllServers()) {
        // check if the server supports the group
        // --> if no: continue
        bool containsGroup = false;
        for (auto group : server.groups) {
            if (group == g) {
                containsGroup = true;
                break;
            }
        }
        if (!containsGroup) {
            continue;
        }
        // check if the servers country already occured
        // --> if yes: continue
        bool countryAlreadyInserted = false;
        for (const auto &country : countries) {
            if (country.id == server.countryId) {
                countryAlreadyInserted = true;
                break;
            }
        }
        if (countryAlreadyInserted) {
            continue;
        }
        // search the country to return
        Nullable<Country> c = this->getCountryById(server.countryId);
        if (c.isNotNull()) {
            countries.push_back(c.value());
        }
    }
    return std::move(countries);
}

void CountryController::notifySubscriber(ICountriesSubscription &subscriber) {
    subscriber.updateCountryList(this->_allCountries);
}

void CountryController::_backgroundTask(bool isSpecialTick) {
    // update the cache the first time (on app start) and then only every
    // 24h (it's an unlikely use case, that the list of countries changes)
    // See registerBackgroundTask(...) to change the isSpecialTick interval.
    this->getAllCountries(isSpecialTick);
}
