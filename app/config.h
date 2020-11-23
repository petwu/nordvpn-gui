#ifndef COMMANDS_H
#define COMMANDS_H

#include <chrono>
#include <filesystem>
#include <pwd.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

// clang-format off

namespace config {

    namespace consts {

        static const uint8_t RATING_MIN           = 1;
        static const uint8_t RATING_MAX           = 5;
        static const uint8_t MAX_RECENTS_REMEMBER = 20;
        static const uint8_t MAX_RECENTS_DISPLAY  = 5;

        static const std::chrono::duration STATUS_UPDATE_INTERVAL           = std::chrono::seconds(1);
        static const std::chrono::duration COUNTRY_LIST_UPDATE_INTERVAL     = std::chrono::seconds(2);
        static const std::chrono::duration SERVER_LIST_UPDATE_INTERVAL      = std::chrono::minutes(15);
        static const std::chrono::duration ENV_UPDATE_INTERVAL              = std::chrono::seconds(2);
        static const std::chrono::duration CONNECTION_COMMANDS_PAUSE_PERIOD = std::chrono::seconds(30);
        static const std::chrono::duration RATING_PERIOD                    = std::chrono::minutes(1);

    } // namespace consts

    namespace paths {

        static const std::filesystem::path getHome() {
            struct passwd *pw = getpwuid(getuid());
            std::string home(pw->pw_dir);
            return std::filesystem::path(home);
        }

        static const std::filesystem::path APP_DIR              = getHome() / ".nordvpn-gui";
        static const std::filesystem::path COUNTRIES_JSON       = std::filesystem::path("..") / "res" / "data" / "countries.json";
        static const std::filesystem::path RECENTS_JSON         = APP_DIR / "recents.json";
        static const std::filesystem::path COUNTRIES_CACHE_JSON = APP_DIR / "countries.cache.json";
        static const std::filesystem::path SERVERS_CACHE_JSON   = APP_DIR / "servers.cache.json";

    } // namespace paths

    namespace urls {

        static const std::string NORDVPN_API_ALL_SERVERS   = "https://api.nordvpn.com/v1/servers?limit=-1";
        static const std::string NORDVPN_API_ALL_COUNTRIES = "https://api.nordvpn.com/v1/servers/countries";

    } // namespace urls

} // namespace config

#endif // COMMANDS_H
