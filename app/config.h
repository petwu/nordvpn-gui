#ifndef COMMANDS_H
#define COMMANDS_H

#include <chrono>
#include <string>

namespace config {

    namespace cmd {

        static const std::string VERSION = "nordvpn --version";
        static const std::string STATUS = "nordvpn status";
        static const std::string RATE = "nordvpn rate";
        static const std::string CITIES = "nordvpn cities";
        static const std::string COUTRIES = "nordvpn countries";
        static const std::string GROUPS = "nordvpn groups";

    } // namespace cmd

    namespace consts {

        static const uint8_t RATING_MIN = 1;
        static const uint8_t RATING_MAX = 5;
        static const std::chrono::duration STATUS_UPDATE_INTERVAL =
            std::chrono::milliseconds(500);

    } // namespace consts

    namespace paths {

        static const std::string COUNTRIES_JSON = "../res/data/countries.json";

    } // namespace paths

} // namespace config

#endif // COMMANDS_H
