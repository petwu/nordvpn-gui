#include "nordvpn/enums/connectionstatus.hpp"

auto connectionStatusToString(ConnectionStatus s) -> std::string {
    switch (s) {
    case ConnectionStatus::Connected:
        return "Connected";
    case ConnectionStatus::Connecting:
        return "Connecting";
    case ConnectionStatus::Disconnected:
        return "Disconnected";
    default:
        return "";
    }
}
