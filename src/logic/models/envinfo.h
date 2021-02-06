#ifndef ENVINFO_H
#define ENVINFO_H

#include <string>

#include "common/types/nullable.h"

/**
 * @brief The EnvInfo class is a data class that accumulates different values
 * about the environment.
 */
class EnvInfo {
  public:
    /**
     * @brief Boolean whether the device has an active internet connection or
     * not.
     */
    bool internetConnected = false;

    /**
     * @brief Boolean whether the device allows to execute shell commands
     * ( `/bin/sh -c "command"` ).
     * @note For almost all Unix system this value is quite unlikely to be
     * false.
     */
    bool shellAvailable = false;

    /**
     * @brief Boolean whether the NordVPN CLI application (`nordvpn`) is
     * installed and available through the PATH environment variable.
     */
    bool nordvpnInstalled = false;

    /**
     * @brief Boolen whether the user is logged in to the NordVPN CLI
     * application or not.
     */
    Nullable<bool> loggedIn;

    /**
     * @brief This string contains the error message returned be "nordvpn
     * status".
     *
     * @details This string is intended to be displayed to the user in case of
     * an unspecified error. In case everything is alright ("nordvpn status"
     * returns code 0), the value is an empty string. The purpose of this is to
     * provided some information about an error case that is not covered by any
     * of the other properties. E.g. it might happen, that "nordvpn status"
     * returns something like this:
     *
     * "Whoops! Cannot reach System Daemon."
     *
     * This might be fixable by restarting the nordvpn daemon with something
     * like "systemctl restart nordvpnd", but it might also be an error caused
     * by a faulty update like in this thread:
     * https://www.reddit.com/r/nordvpn/comments/kfewlb/solution_whoops_cannot_reach_system_daemon_after
     * Therefore, the cause of the error is not being interpreted any further.
     */
    std::string miscError;

    /**
     * @brief Convert the object into a printable, nice-to-read string.
     */
    auto toString() const -> std::string;
};

#endif // ENVINFO_H
