#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <chrono>

namespace nordvpn {

    static const std::string CMD_VERSION = "nordvpn --version";
    static const std::string CMD_STATUS = "nordvpn status";
    static const std::string CMD_RATE = "nordvpn rate";
    static const std::string CMD_CITIES = "nordvpn cities";
    static const std::string CMD_COUTRIES = "nordvpn countries";
    static const std::string CMD_GROUPS = "nordvpn groups";

    static const uint8_t RATING_MIN = 1;
    static const uint8_t RADING_MAX = 5;

    static const std::chrono::duration STATUS_UPDATE_INTERVAL = std::chrono::milliseconds(500);

} // namespace nordvpn

#endif // COMMANDS_H