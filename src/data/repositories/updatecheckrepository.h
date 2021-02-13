#ifndef UPDATECHECKREPOSITORY_H
#define UPDATECHECKREPOSITORY_H

#include "baserepository.h"
#include "common/types/version.h"

/**
 * @brief The UpdateCheckRepository class provides functions to retrieve version
 * information about the application and the newest availabe version. Those can
 * be used to determine whether an update is available or not.
 */
class UpdateCheckRepository : public BaseRepository {
  public:
    /**
     * @brief Returns the Version that was compiled into the application. For a
     * single built this version remains the same.
     */
    static auto getCurrentVersion() -> Version;

    /**
     * @brief Calls the GitHub API to retrieve the version number of the latest
     * release.
     * @return Version::Invalid, if something went wrong (HTTP request failed,
     * response not parsable, etc.).
     */
    static auto getLatestVersion() -> Version;
};

#endif // UPDATECHECKREPOSITORY_H
