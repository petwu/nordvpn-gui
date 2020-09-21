#ifndef UTIL_STRINGS_H
#define UTIL_STRINGS_H

#include <algorithm>
#include <cctype>
#include <locale>
#include <string>
#include <vector>

namespace util {

    namespace string {

        std::string trim(std::string &s);
        std::string trimPrefix(std::string &s);
        std::string trimSuffix(std::string &s);

        std::vector<std::string> split(std::string s, std::string delim);

    } // namespace string

} // namespace util

#endif
