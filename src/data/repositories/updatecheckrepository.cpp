#include "updatecheckrepository.h"

#include <string>

#include "app.h"

auto UpdateCheckRepository::getCurrentVersion() -> Version {
    return Version(SEMVER_MAJOR,       //
                   SEMVER_MINOR,       //
                   SEMVER_PATCH,       //
                   SEMVER_PRE_RELEASE, //
                   SEMVER_BUILD_META);
}

// clang-format off

/**
 * @brief GITHUB_API_LATEST_RELEASE
 * @details API endpoint that returns the latest release: [GET] /repos/{owner}/{repo}/releases/latest
 * Parameters:
 *
 * Name     | Type    | In      | Description
 * ---------|---------|---------|----------------------------------------------------------.
 * accept   | string  | header  | Setting to application/vnd.github.v3+json is recommended.
 * owner    | string  | path    |
 * repo     | string  | path    |
 *
 * @see https://docs.github.com/en/rest/reference/repos#get-the-latest-release
 */
const auto GITHUB_API_ALL_RELEASES = std::string("https://api.github.com/repos/") + GITHUB_USERNAME + "/" + GITHUB_REPOSITORY_NAME + "/releases";
//const auto GITHUB_API_LATEST_RELEASE = GITHUB_API_ALL_RELEASES + "/latest";
const auto GITHUB_API_LATEST_RELEASE  = "https://api.github.com/repos/Kitware/CMake/releases";

/**
 * @brief HEADER_USER_AGENT
 * @details GitHub API v3 requires that a user agent is set.
 * The documentation recommend using the user and repository name instead of a typical browser user agent.
 * @see https://docs.github.com/en/rest/overview/resources-in-the-rest-api#user-agent-required
 */
const auto HEADER_USER_AGENT = std::string("User-Agent: ") + GITHUB_USERNAME + "/" + GITHUB_REPOSITORY_NAME;

/**
 * @brief HEADER_ACCEPT
 * @details The Accept header is used to specify the API version.
 * @see #GITHUB_API_LATEST_RELEASE
 */
const auto HEADER_ACCEPT = std::string("Accept: application/vnd.github.v3+json");

// clang-format on

auto UpdateCheckRepository::getLatestVersion() -> Version {
    // call GitHub API for release information
    std::string httpReponse =
        BaseRepository::curl(GITHUB_API_LATEST_RELEASE, DEFAULT_CURL_TIMEOUT,
                             {HEADER_USER_AGENT, HEADER_ACCEPT});
    // parse the response
    json j;
    try {
        // empty or non-JSON responses will throw an exception
        j = json::parse(httpReponse);
    } catch (...) {
        return Version::Invalid;
    }
    // The response must be an object with a property "tag_name" that contains
    // the release tag. The tag should be a version string that can be parsed by
    // the Version object.
    if (j.is_object() && j["tag_name"].is_string()) {
        return Version::fromString(j["tag_name"]);
    }
    return Version::Invalid;
}
