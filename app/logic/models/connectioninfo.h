#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <string>
#include <vector>

#include "data/enums/group.h"
#include "data/enums/protocol.h"
#include "data/enums/technology.h"
#include "logic/enums/ConnectionStatus.h"

/**
 * @brief The ConnectionInfo class is a data class that accumulates all
 * information about the current connection and server.
 */
class ConnectionInfo {
  public:
    /**
     * @brief Default constructor.
     */
    ConnectionInfo() = default;

    /**
     * @brief Copy constructor.
     */
    ConnectionInfo(const ConnectionInfo &c) = default;

    /**
     * @brief Copy assignment.
     */
    ConnectionInfo &operator=(const ConnectionInfo &) = default;

    /**
     * @brief Move constructor.
     */
    ConnectionInfo(ConnectionInfo &&m) = default;

    /**
     * @brief Move assignment.
     */
    ConnectionInfo &operator=(ConnectionInfo &&) = default;

    /**
     * @brief The current connection status. If it is
     * ConnectionStatus::Disconnected, all other attributes may be ignored since
     * they should have their initial zero value.
     */
    ConnectionStatus status = ConnectionStatus::Disconnected;

    /**
     * @brief The domain name of the connected server. Example:
     * `de123.nordvpn.com`.
     */
    std::string server = "";

    /**
     * @brief The servers number. This information reflects the number in the
     * `server` attribute. Example: If `server == "de123.nordvpn.com"` then
     * `serverNr` will be `123`.
     */
    int32_t serverNr = 0;

    /**
     * @brief The name of the country the server is located in. Examples:
     * Germay, United States, Switzerland, etc.
     */
    std::string country = "";

    /**
     * @brief The countries ID according to the NordVPN API.
     * @see https://api.nordvpn.com/v1/servers/countries
     */
    int32_t countryId = -1;

    /**
     * @brief The name of the city the server is located in. Examples:
     * Frankfurt, New York, Zurich, etc.
     */
    std::string city = "";

    /**
     * @brief The IP address of the server and therefore the users public IP as
     * well. Example: 195.181.170.199
     */
    std::string ip = "";

    /**
     * @brief The VPN technology used to establish the connection.
     */
    Technology technology = Technology::Undefined;

    /**
     * @brief The network protocol used for network traffic.
     * @details This property might be set Protocol::Undefined depending on the
     * value of #technology (e.g. Technology::NordLynx).
     */
    Protocol connectionType = Protocol::Undefined;

    /**
     * @brief The number of bytes sent while connected to this server.
     */
    uint64_t sent = 0;

    /**
     * @brief The number of bytes received while connected to this server.
     */
    uint64_t received = 0;

    /**
     * @brief The number of seconds connected to this server.
     */
    uint64_t uptime = 0;

    /**
     * @brief A list of server groups this server is part of.
     */
    std::vector<Group> groups;

    /**
     * @brief A number denoting how much of the servers resources are used. The
     * value is in percent and therefore between 0 and 100.
     * @note This value is a snapshot of the load rate at a given point that is
     * note updated as frequently as the ConnectionInfo. See
     * #config::consts::SERVER_LIST_UPDATE_INTERVAL for the actual update
     * interval.
     */
    uint8_t load;

    /**
     * @brief Checks whether the object is empty aka. in it's initial state.
     * @return True, if all attributes still have their zero value. False, if
     * one or more attributes were assigned a different value.
     */
    bool isEmpty() const;
};

#endif // CONNECTIONINFO_H
