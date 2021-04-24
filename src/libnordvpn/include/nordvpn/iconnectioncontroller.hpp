#ifndef ICONNECTIONCONTROLLER_HPP
#define ICONNECTIONCONTROLLER_HPP

#include <cstdint>
#include <sys/types.h>

#include "nordvpn/enums/group.hpp"
#include "nordvpn/models/connectioninfo.hpp"

/**
 * @brief The IConnectionController interface provides functions to establish
 * and disconnect connections to a VPN server.
 */
class IConnectionController {
  public:
    /**
     * @brief Connect to a VPN server. Let NordVPN decide which country and
     * server is best.
     */
    virtual void quickConnect() = 0;

    /**
     * @brief Connect to a VPN server in a specific country. Let NordVPN decide
     * which city and server in this country is best.
     */
    virtual void connectToCountryById(uint32_t id) = 0;

    /**
     * @brief Connect to a VPN server in a specific city. Let NordVPN decide
     * which server in this city is best.
     */
    virtual void connectToCityById(uint32_t id) = 0;

    /**
     * @brief Connect to a specific VPN server.
     */
    virtual void connectToServerById(uint32_t id) = 0;

    /**
     * @brief Connect to a VPN server in a specific server #Group. Let NordVPN
     * decide which country and server is best.
     */
    virtual void connectToSpecialtyGroup(Group g) = 0;

    /**
     * @brief Connect to a country within a specific server group. Let NordVPN
     * decide which server in the group and county is best.
     */
    virtual void connectToCountryByIdAndGroup(uint32_t id, Group g) = 0;

    /**
     * @brief Abort an connection attempt.
     * @details This kills the process establishing the connection and performs
     * a disconnect. Hence, if already connected, this function has the same
     * effect as #disconnect().
     */
    virtual void cancelConnection() = 0;

    /**
     * @brief Disconnect from the currently connected VPN server. If not
     * connected, this has no effect.
     */
    virtual void disconnect() = 0;
};

#endif // ICONNECTIONCONTROLLER_HPP
