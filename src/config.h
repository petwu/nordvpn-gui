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

        constexpr uint8_t RATING_MIN           = 1;
        constexpr uint8_t RATING_MAX           = 5;
        constexpr uint8_t MAX_RECENTS_REMEMBER = 20;
        constexpr uint8_t MAX_RECENTS_DISPLAY  = 5;

        constexpr std::chrono::duration STATUS_UPDATE_INTERVAL           = std::chrono::seconds(1);
        constexpr std::chrono::duration COUNTRY_LIST_UPDATE_INTERVAL     = std::chrono::seconds(2);
        constexpr std::chrono::duration SERVER_LIST_UPDATE_INTERVAL      = std::chrono::minutes(15);
        constexpr std::chrono::duration ENV_UPDATE_INTERVAL              = std::chrono::seconds(2);
        constexpr std::chrono::duration CONNECTION_COMMANDS_PAUSE_PERIOD = std::chrono::seconds(30);
        constexpr std::chrono::duration RATING_PERIOD                    = std::chrono::minutes(1);

    } // namespace consts

    namespace paths {

        static auto getHome() -> std::filesystem::path {
            struct passwd *pw = getpwuid(getuid());
            std::string home(pw->pw_dir);
            return std::filesystem::path(home);
        }

        static auto appDir() -> std::filesystem::path {
            return getHome() / ".nordvpn-gui";
        }
        static auto countriesJson() -> std::filesystem::path {
            return std::filesystem::path("..") / "res" / "data" / "countries.json";
        }
        static auto recentsJson() -> std::filesystem::path {
            return appDir() / "recents.json";
        }
        static auto countriesCacheJson() -> std::filesystem::path {
            return appDir() / "countries.cache.json";
        }
        static auto serversCacheJson() -> std::filesystem::path {
            return appDir() / "servers.cache.json";
        }

    } // namespace paths

    namespace urls {

        constexpr const char* NORDVPN_API_ALL_SERVERS   = "https://api.nordvpn.com/v1/servers?limit=-1";
        constexpr const char* NORDVPN_API_ALL_COUNTRIES = "https://api.nordvpn.com/v1/servers/countries";

    } // namespace urls

} // namespace config

#endif // COMMANDS_H
