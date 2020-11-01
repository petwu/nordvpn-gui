#ifndef COMMANDS_H
#define COMMANDS_H

#include <chrono>
#include <filesystem>
#include <pwd.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

namespace config {

    namespace consts {

        static const uint8_t RATING_MIN = 1;
        static const uint8_t RATING_MAX = 5;
        static const std::chrono::duration STATUS_UPDATE_INTERVAL =
            std::chrono::seconds(1);
        static const std::chrono::duration COUNTRY_LIST_UPDATE_INTERVAL =
            std::chrono::seconds(2);
        static const std::chrono::duration SERVER_LIST_UPDATE_INTERVAL =
            std::chrono::minutes(15);
        static const uint8_t MAX_RECENTS_REMEMBER = 20;
        static const uint8_t MAX_RECENTS_DISPLAY = 5;

    } // namespace consts

    namespace paths {

        static const std::filesystem::path getHome() {
            struct passwd *pw = getpwuid(getuid());
            std::string home(pw->pw_dir);
            return std::filesystem::path(home);
        }
        static const std::filesystem::path APP_DIR = getHome() / ".nordvpn-gui";
        static const std::filesystem::path COUNTRIES_JSON =
            std::filesystem::path("..") / "res" / "data" / "countries.json";
        static const std::filesystem::path RECENTS_JSON =
            APP_DIR / "recents.json";
        static const std::filesystem::path COUNTRIES_CACHE_JSON =
            APP_DIR / "countries.cache.json";
        static const std::filesystem::path SERVERS_CACHE_JSON =
            APP_DIR / "servers.cache.json";

    } // namespace paths

    namespace urls {

        /*
         * NordVPN's API (api.nordvpn.com) limits the number of request to one
         * endpoints to 500 per day (tested 2020-10-30). Let's do the math:
         *
         *      Minutes per day:    24 h/d * 60 min/h = 1440 min/d
         *      Minumum period:     1440 min / 500 = 2.88 min
         *
         * It therefore is recommended to do periodic call to an API endpoint
         * with a minimum time period between to requests of 3 min. Higher is of
         * cource better.
         *
         * It is worth mentioning, that calls to the CLI tool like 'nordvpn
         * account' perform an API request as well and hence reduce your
         * remaining quota in the same way a direct API request does.
         *
         */

        static const std::string NORDVPN_API_ALL_SERVERS =
            "https://api.nordvpn.com/v1/servers?limit=-1";
        static const std::string NORDVPN_API_ALL_COUNTRIES =
            "https://api.nordvpn.com/v1/servers/countries";

    } // namespace urls

} // namespace config

#endif // COMMANDS_H
