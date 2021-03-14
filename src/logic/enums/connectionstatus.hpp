#ifndef CONNECTIONSTATUS_HPP
#define CONNECTIONSTATUS_HPP

#include <string>

/**
 * @brief The ConnectionStatus enum defines values for representing general
 * connection state to a VPN server.
 */
enum class ConnectionStatus {
    Disconnected, ///< Not connected to any VPN server.
    Connecting,   ///< The connection to a VPN server is being established.
    Connected,    ///< Connected to a VPN server.
};

/**
 * @brief Convert a ConnectionStatus enum to a string.
 */
auto connectionStatusToString(ConnectionStatus s) -> std::string;

#endif // CONNECTIONSTATUS_HPP
