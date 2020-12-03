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
     * @brief Convert the object into a printable, nice-to-read string.
     */
    auto toString() const -> std::string;
};

#endif // ENVINFO_H
