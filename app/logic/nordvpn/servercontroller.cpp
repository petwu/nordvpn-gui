#include "servercontroller.h"

json::array_t ServerController::getAllCountries() {
    auto cmdResult = execute(config::cmd::COUTRIES);
    auto availableCountries = util::string::split(cmdResult.output, ", ");
    json all = ServerRepository::getCountriesJSON();
    json available = json::array();
    // filter the json array to only contains countries that were returned by
    // the CLI command
    for (std::string c : availableCountries) {
        for (auto e : all) {
            if (c == e["connectName"]) {
                available += e;
            }
        }
    }
    return std::move(available);
}

std::vector<std::string> ServerController::getCities(std::string country) {
    auto result = execute(config::cmd::CITIES + " " + country);
    return util::string::split(result.output, ", ");
}

std::vector<std::string> ServerController::getGroups() {
    auto result = execute(config::cmd::GROUPS);
    return util::string::split(result.output, ", ");
}
