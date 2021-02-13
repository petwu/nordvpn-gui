#ifndef VERSION_H
#define VERSION_H

#include <cstdint>
#include <string>

/**
 * @brief The Version class represents a version number according to semantic
 * versioning (https://semver.org).
 *
 * @details Version overrides operators (==, !=, >, <, >=, <=) to compare two
 * Version objects according to the semver specification. Parsing a Version
 * object from a string is possible as well.
 */
class Version {
  public:
    /**
     * @brief Default constructor. "<major>.<minor>.<patch>" information is
     * required. Either a pre-release or build metadata or both are optional.
     * See https://semver.org for more details.
     *
     * @details The constructor checks that preRelease and buildMeta are valid
     * according to the semver specification (dot separated alphanumberic values
     * with hyphens; roughly: [1-9a-zA-Z][0-0a-zA-Z-]*(\.[0-0a-zA-Z-]+)). Refer
     * to https://semver.org for a full BNF grammer. If the values are invalid,
     * the constructor assigns Version::Invalid to the new object instead of the
     * constructor parameters.
     */
    Version(uint32_t major, uint32_t minor, uint32_t patch,
            std::string preRelease = "", std::string buildMeta = "");

    /**
     * @brief Represents a invalud version. #toString() returns "0.0.0-invalid".
     * Can be used in equal comparisons to check if a Version object hold a
     * valid version, e.g. `if (myVersion == Version::Invalid) { ... }` or
     * `if (myVersion != Version::Invalid) { ... }`.
     */
    static Version Invalid;

    /**
     * @brief Parses a string to a Version object.
     *
     * @details The string is validated using regular expressions to conform to
     * the semver specification. If the string is not a valid version according
     * to semver, Version::Invalid is returned.
     *
     * @note Semver formally does not allow a "v" or "V" prefix, e.g. "v1.0.0".
     * Here this function is less strict than the official specification and
     * simply ignores a leading v|V. This is for convenience reasons, because
     * tags in version control systems like Git are often prefixed with v|V.
     */
    static auto fromString(std::string s) -> Version;

    /**
     * @brief Returns a semver string, e.g. "1.0.0", "1.1.0-alpha", "1.2.0-rc2",
     * "2.0.0-dev", "1.1.0-alpha+build001", etc.
     */
    auto toString() const -> std::string;

    auto operator==(const Version &other) const -> bool;
    auto operator!=(const Version &other) const -> bool;
    auto operator>(const Version &other) const -> bool;
    auto operator<(const Version &other) const -> bool;
    auto operator>=(const Version &other) const -> bool;
    auto operator<=(const Version &other) const -> bool;

  private:
    /**
     * @details The empty default constructor is required by #fromString(), but
     * should not be used by anyone to enforce validation checks through the
     * constructor. Hence it is private.
     */
    Version() = default;

    /** @brief <MAJOR>.<minor>.<patch>-<pre-release>+<build-metadata> */
    uint32_t _major = 0;
    /** @brief <major>.<MINOR>.<patch>-<pre-release>+<build-metadata> */
    uint32_t _minor = 0;
    /** @brief <major>.<minor>.<PATCH>-<pre-release>+<build-metadata> */
    uint32_t _patch = 0;
    /** @brief <major>.<minor>.<patch>-<PRE-RELEASE>+<build-metadata> */
    std::string _preRelease;
    /** @brief <major>.<minor>.<patch>-<pre-release>+<BUILD-METADATA> */
    std::string _buildMeta;
};

#endif // VERSION_H
