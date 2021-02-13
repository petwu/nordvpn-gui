#include "version.h"

#include <regex>

#include "common/util/strings.h"

Version::Version(uint32_t major, uint32_t minor, uint32_t patch,
                 std::string preRelease, std::string buildMeta)
    : _major(major), _minor(minor), _patch(patch), _preRelease(preRelease),
      _buildMeta(buildMeta) {
    // Simply assigning major, minor, etc. to is appropriate private fields does
    // not prevent invald information passed to preRelease and buildMeta. In
    // order to not duplicate logic, we re-use the validation implemented in
    // fromString(). fromString() uses the (private) empty default constructor
    // to avoid enless loops.
    *this = Version::fromString(this->toString());
}

Version Version::Invalid = Version(0, 0, 0, "invalid");

auto Version::fromString(std::string s) -> Version {
    if (s.empty()) {
        return Version::Invalid;
    }
    // the semver specification does not allow a leading v|V for "version", but
    // we are a bit less strict
    if (s[0] == 'v' || s[0] == 'V') {
        s = s.substr(1);
    }
    // the regex is taken directly from the senver website:
    // https://semver.org/#is-there-a-suggested-regular-expression-regex-to-check-a-semver-string
    auto semverRegex = std::regex(
        R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)");
    std::smatch matches;
    if (!std::regex_search(s, matches, semverRegex)) {
        return Version::Invalid;
    }
    // std::stoi() can throw exceptions, but we don't have to catch it since we
    // already checked with a regex
    Version v;
    v._major = std::stoi(matches[1]);
    v._minor = std::stoi(matches[2]);
    v._patch = std::stoi(matches[3]);
    v._preRelease = matches[4];
    v._buildMeta = matches[5];
    return v;
}

auto Version::toString() const -> std::string {
    auto version = std::to_string(this->_major) + "." +
                   std::to_string(this->_minor) + "." +
                   std::to_string(this->_patch);
    if (!this->_preRelease.empty()) {
        version += "-" + this->_preRelease;
    }
    if (!this->_buildMeta.empty()) {
        version += "+" + this->_buildMeta;
    }
    return version;
}

auto Version::operator==(const Version &other) const -> bool {
    return this->_major == other._major && //
           this->_minor == other._minor && //
           this->_patch == other._patch && //
           this->_preRelease == other._preRelease;
}

auto Version::operator!=(const Version &other) const -> bool {
    return !(*this == other);
}

auto Version::operator>(const Version &other) const -> bool {
    if (*this == Version::Invalid) {
        return false;
    } else if (other == Version::Invalid) {
        return true;
    }

    // [semver spec 11.1] Precedence MUST be calculated by separating the
    // version into major, minor, patch and pre-release identifiers in that
    // order (Build metadata does not figure into precedence).
    // [semver spec 11.2] Precedence is determined by the first difference when
    // comparing each of these identifiers from left to right as follows: Major,
    // minor, and patch versions are always compared numerically.
    // Example: 1.0.0 < 2.0.0 < 2.1.0 < 2.1.1.

    if (this->_major < other._major || // if one of this is true, _this_ can't
        this->_minor < other._minor || // have higher precedence than _other_
        this->_patch < other._patch) {
        return false;
    }
    if (this->_major > other._major || // if major is bigger, _this_ has
        this->_minor > other._minor || // precedence, otherwise major is equal;
        this->_patch > other._patch) { // same logic for minor and patch
        return true;
    }

    // [semver spec 11.3] When major, minor, and patch are equal, a pre-release
    // version has lower precedence than a normal version:

    if (this->_preRelease.empty() && !other._preRelease.empty()) {
        return true;
    } else if (!this->_preRelease.empty() && other._preRelease.empty()) {
        return false;
    }

    // [semver spec 11.4] Precedence for two pre-release versions with the same
    // major, minor, and patch version MUST be determined by comparing each dot
    // separated identifier from left to right until a difference is found as
    // follows:
    // 1. Identifiers consisting of only digits are compared numerically.
    // 2. Identifiers with letters or hyphens are compared lexically in ASCII
    //    sort order.
    // 3. Numeric identifiers always have lower precedence than non-numeric
    //    identifiers.
    // 4. A larger set of pre-release fields has a higher precedence than a
    //    smaller set, if all of the preceding identifiers are equal.
    // Example: 1.0.0-alpha < 1.0.0-alpha.1 < 1.0.0-alpha.beta < 1.0.0-beta
    // < 1.0.0-beta.2 < 1.0.0-beta.11 < 1.0.0-rc.1 < 1.0.0.

    std::vector<std::string> tParts =
        util::string::split(this->_preRelease, ".");
    std::vector<std::string> oParts =
        util::string::split(other._preRelease, ".");
    for (size_t i = 0; i < tParts.size() && i < oParts.size(); i++) {
        if (tParts[i] == oParts[i]) {
            continue; // possibly 11.4.4
        }
        auto numberRegex = std::regex("-?[0-9]");
        bool tPartIsNumeric = std::regex_match(tParts[i], numberRegex);
        bool oPartIsNumeric = std::regex_match(oParts[i], numberRegex);
        if (tPartIsNumeric && oPartIsNumeric) {
            // std::stoi() can throw exceptions, but we don't have to catch it
            // since we already checked with a regex
            int tPartNum = std::stoi(tParts[i], 0, 10);
            int oPartNum = std::stoi(oParts[i], 0, 10);
            if (tPartNum != oPartNum) {
                return tPartNum > oPartNum; // 11.4.1
            }
        } else if (tPartIsNumeric) {
            return false; // 11.4.3
        } else if (oPartIsNumeric) {
            return true; // 11.4.3
        } else {
            return tParts[i] > oParts[i]; // 11.4.2
        }
    }
    return tParts.size() > oParts.size(); // 11.4.4
}

auto Version::operator<(const Version &other) const -> bool {
    return other > *this;
}

auto Version::operator>=(const Version &other) const -> bool {
    return *this > other || *this == other;
}
auto Version::operator<=(const Version &other) const -> bool {
    return *this < other || *this == other;
}
