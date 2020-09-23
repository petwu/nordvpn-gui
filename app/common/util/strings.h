#ifndef UTIL_STRINGS_H
#define UTIL_STRINGS_H

#include <algorithm>
#include <cctype>
#include <iostream>
#include <locale>
#include <regex>
#include <string>
#include <vector>

namespace util {

    namespace string {

        std::string trim(const std::string &s);
        std::string trimPrefix(const std::string &s);
        std::string trimSuffix(const std::string &s);

        std::vector<std::string> split(const std::string &s,
                                       const std::string &delim);

        std::string toLower(const std::string &s);
        std::string toUpper(const std::string &s);

        std::string replaceAll(const std::string &s, const std::string &oldSub,
                               const std::string &newSub);

    } // namespace string

} // namespace util

#endif
