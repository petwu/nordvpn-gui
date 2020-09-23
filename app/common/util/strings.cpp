#include "strings.h"

namespace util {

    namespace string {

        std::string trim(const std::string &s) {
            std::string r(s);
            r = trimPrefix(r);
            r = trimSuffix(r);
            return std::move(r);
        }

        std::string trimPrefix(const std::string &s) {
            std::string r(s);
            r.erase(r.begin(),
                    std::find_if(r.begin(), r.end(), [](unsigned char c) {
                        return !std::isspace(c) && c != '-';
                    }));
            return std::move(r);
        }

        std::string trimSuffix(const std::string &s) {
            std::string r(s);
            r.erase(
                std::find_if(r.rbegin(), r.rend(),
                             [](unsigned char c) { return !std::isspace(c); })
                    .base(),
                r.end());
            return std::move(r);
        }

        std::vector<std::string> split(const std::string &s,
                                       const std::string &delim = " ") {
            std::vector<std::string> parts;
            size_t cur, prev = 0;
            cur = s.find(delim);
            while (cur != std::string::npos) {
                parts.push_back(s.substr(prev, cur - prev));
                prev = cur + delim.length();
                cur = s.find(delim, prev);
            }
            parts.push_back(s.substr(prev, cur - prev));
            return std::move(parts);
        }

        std::string toLower(const std::string &s) {
            std::string r;
            for (const char &c : s) {
                r += std::tolower(c);
            }
            return std::move(r);
        }

        std::string toUpper(const std::string &s) {
            std::string r;
            for (const char &c : s) {
                r += std::toupper(c);
            }
            return std::move(r);
        }

        std::string replaceAll(const std::string &s, const std::string &oldSub,
                               const std::string &newSub) {
            return std::regex_replace(s, std::regex(oldSub), newSub);
        }

    } // namespace string

} // namespace util
