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
        static const std::string CONNECT = "nordvpn connect";
        static const std::string DISCONNECT = "nordvpn disconnect";

    } // namespace cmd

    namespace consts {

        static const uint8_t RATING_MIN = 1;
        static const uint8_t RATING_MAX = 5;
        static const std::chrono::duration STATUS_UPDATE_INTERVAL =
            std::chrono::milliseconds(1000);

    } // namespace consts

    namespace paths {

        static const std::string COUNTRIES_JSON = "../res/data/countries.json";

    } // namespace paths

    namespace urls {

        static const std::string NORDVPN_API_ALL_SERVERS =
            "https://api.nordvpn.com/v1/servers?limit=-1";

    }

} // namespace config

#endif // COMMANDS_H
