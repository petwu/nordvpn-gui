#ifndef IUPDATECHECKREPOSITORY_HPP
#define IUPDATECHECKREPOSITORY_HPP

#include "nordvpn/models/version.hpp"

/**
 * @brief The IUpdateCheckRepository interface provides functions to retrieve
 * version information about the application and the newest availabe version.
 * Those can be used to determine whether an update is available or not.
 */
class IUpdateCheckRepository {
  public:
    /**
     * @brief Returns the Version that was compiled into the application. For a
     * single built this version remains the same.
     */
    virtual auto getCurrentVersion() -> Version = 0;

    /**
     * @brief Calls the GitHub API to retrieve the version number of the latest
     * release.
     * @return Version::Invalid, if something went wrong (HTTP request failed,
     * response not parsable, etc.).
     */
    virtual auto getLatestVersion() -> Version = 0;
};

#endif // IUPDATECHECKREPOSITORY_HPP
