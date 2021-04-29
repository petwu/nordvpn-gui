#include "updatecheckrepository.hpp"

#include <algorithm>
#include <initializer_list>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "baserepository.hpp"
#include "libnordvpn.hpp"
#include "nordvpn/config.hpp"
#include "nordvpn/models/version.hpp"

UpdateCheckRepository::UpdateCheckRepository()
    : _hiddenUpdatesJsonPath(config::paths::hiddenUpdatesJson()) {}

auto UpdateCheckRepository::getCurrentVersion() -> Version {
    return Version(
        SEMVER_MAJOR,      //
        SEMVER_MINOR,      //
        SEMVER_PATCH,      //
        SEMVER_PRE_RELEASE //
        /*, SEMVER_BUILD_META <- not of interest for an update notification */);
}

auto UpdateCheckRepository::getAllReleaseVersions() -> std::vector<Version> {
    // call GitHub API for release information
    // API endpoint doc:
    // https://docs.github.com/en/rest/reference/repos#get-the-latest-release
    // https://docs.github.com/en/rest/overview/resources-in-the-rest-api#user-agent-required
    auto url = std::string("https://api.github.com/repos/") + GITHUB_USERNAME +
               "/" + GITHUB_REPOSITORY_NAME + "/releases";
    auto userAgent = std::string("User-Agent: ") + GITHUB_USERNAME + "/" +
                     GITHUB_REPOSITORY_NAME;
    auto accept = std::string("Accept: application/vnd.github.v3+json");
    std::string httpReponse =
        BaseRepository::curl(url, DEFAULT_CURL_TIMEOUT, {userAgent, accept});
    // parse the response
    json j;
    std::vector<Version> releases;
    try {
        // empty or non-JSON responses will throw an exception
        j = json::parse(httpReponse);
    } catch (...) {
        return releases;
    }
    // The response must be an array of objects with a "tag_name" property that
    // contains the release tag. The tag should be a version string that can be
    // parsed by the Version object.
    if (j.is_array()) {
        for (auto &item : j) {
            if (!item.is_object() || !j["tag_name"].is_string()) {
                continue;
            }
            auto version = Version::fromString(j["tag_name"]);
            if (version.isValid()) {
                releases.push_back(version);
            }
        }
    }
    std::sort(releases.begin(), releases.end()); // < operator is overridden
    return releases;
}

void UpdateCheckRepository::setIsUpdateHidden(Version v, bool isHidden) {
    // remembering to hide the update notification for a specific version is
    // achived by storing all versions v in an array in a JSON file
    std::string file = BaseRepository::readFile(this->_hiddenUpdatesJsonPath);
    json currentArray;
    json updatedArray = json::array();
    // if the file does not exists or is empty, then json::parse() throws an
    // exception
    try {
        currentArray = json::parse(file);
    } catch (...) {
    }
    // filter out v and then only add it again if isHidden is true;
    // this assures a corrent result and prevents duplicates at the same time
    if (currentArray.is_array()) {
        for (auto &elem : currentArray) {
            if (!elem.is_string() || json::string_t(elem) == v.toString()) {
                continue;
            }
            updatedArray.push_back(elem);
        }
    }
    if (isHidden) {
        updatedArray.push_back(v.toString());
    }
    // write the updated array back to file
    UpdateCheckRepository::writeFile(this->_hiddenUpdatesJsonPath,
                                     updatedArray.dump(2));
}

auto UpdateCheckRepository::isUpdateHidden(Version v) -> bool {
    std::string file =
        UpdateCheckRepository::readFile(this->_hiddenUpdatesJsonPath);
    json j;
    try {
        j = json::parse(file);
    } catch (...) {
        return false;
    }
    if (!j.is_array()) {
        return false;
    }
    return std::any_of(j.begin(), j.end(), [&v](const json &elem) {
        return elem.is_string() && json::string_t(elem) == v.toString();
    });
}
