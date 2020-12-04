#ifndef UTIL_STRINGS_H
#define UTIL_STRINGS_H

#include <string>
#include <vector>

namespace util {

    namespace string {

        auto trim(const std::string &s) -> std::string;
        auto trimPrefix(const std::string &s) -> std::string;
        auto trimSuffix(const std::string &s) -> std::string;

        auto split(const std::string &s, const std::string &delim)
            -> std::vector<std::string>;

        auto toLower(const std::string &s) -> std::string;
        auto toUpper(const std::string &s) -> std::string;

        auto replaceAll(const std::string &s, const std::string &oldSub,
                        const std::string &newSub) -> std::string;
        auto replacePrefix(const std::string &s, const std::string &prefix,
                           const std::string &replacement) -> std::string;
        auto replaceSuffix(const std::string &s, const std::string &suffix,
                           const std::string &replacement) -> std::string;

    } // namespace string

} // namespace util

#endif
