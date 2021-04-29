#ifndef IUPDATECHECKREPOSITORY_HPP
#define IUPDATECHECKREPOSITORY_HPP

#include <vector>

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
     * @brief Calls the GitHub API to retrieve the version number of all
     * releases.
     * @return An array of version numbers. They are sorted from lowest to
     * highest according to Semantic Versioning and the < operator of #Version.
     * An empty vector, if something went wrong (HTTP request failed, response
     * not parsable, etc.) or no releases exists yet.
     */
    virtual auto getAllReleaseVersions() -> std::vector<Version> = 0;

    /**
     * @brief Remembers persistently that the user does not want a notification
     * about avaiable updates when the specified version is the latest
     * non-hidden version.
     */
    virtual void setIsUpdateHidden(Version v, bool isHidden) = 0;

    /**
     * @brief Checks whether #hideUpdate() was previously called for the
     * specified Version `v`.
     */
    virtual auto isUpdateHidden(Version v) -> bool = 0;
};

#endif // IUPDATECHECKREPOSITORY_HPP
