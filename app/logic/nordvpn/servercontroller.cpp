#include "servercontroller.h"

json::array_t ServerController::getAllCountries() {
    if (this->_allCountries.is_null()) {
        auto cmdResult = execute(config::cmd::COUTRIES);
        auto availableCountries = util::string::split(cmdResult.output, ", ");
        json all = ServerRepository::getCountriesJSON();
        json available = json::array();
        // filter the json array to only contains countries that were returned
        // by the CLI command
        for (std::string c : availableCountries) {
            for (auto e : all) {
                if (c == e["connectName"]) {
                    available += e;
                }
            }
        }
        this->_allCountries = available;
    }
    return this->_allCountries;
}

std::vector<std::string> ServerController::getCities(std::string country) {
    auto result = execute(config::cmd::CITIES + " " + country);
    return util::string::split(result.output, ", ");
}

std::vector<std::string> ServerController::getGroups() {
    auto result = execute(config::cmd::GROUPS);
    return util::string::split(result.output, ", ");
}

void ServerController::quickConnect() {
    this->executeNonBlocking(config::cmd::CONNECT);
}

void ServerController::connectToCountryById(uint32_t id) {
    for (auto country : this->_allCountries) {
        if (country["id"] == id && country["connectName"].is_string()) {
            this->executeNonBlocking(config::cmd::CONNECT + " " +
                                     std::string(country["connectName"]));
            return;
        }
    }
}

void ServerController::disconnect() {
    this->executeNonBlocking(config::cmd::DISCONNECT);
}
