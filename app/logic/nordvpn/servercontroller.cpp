#include "servercontroller.h"

std::vector<std::string> ServerController::getAllCountries() {
    auto result = execute(nordvpn::CMD_COUTRIES);
    return util::string::split(result->output, ", ");
}

std::vector<std::string> ServerController::getCities(std::string country) {
    auto result = execute(nordvpn::CMD_CITIES + " " + country);
    return util::string::split(result->output, ", ");
}

std::vector<std::string> ServerController::getGroups() {
    auto result = execute(nordvpn::CMD_GROUPS);
    return util::string::split(result->output, ", ");
}
