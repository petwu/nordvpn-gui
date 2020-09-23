#include "strings.h"

namespace util {

    namespace string {

        std::string trim(std::string &s) {
            std::string r(s);
            r = trimPrefix(r);
            r = trimSuffix(r);
            return r;
        }

        std::string trimPrefix(std::string &s) {
            std::string r(s);
            r.erase(r.begin(),
                    std::find_if(r.begin(), r.end(), [](unsigned char c) {
                        return !std::isspace(c) && c != '-';
                    }));
            return r;
        }

        std::string trimSuffix(std::string &s) {
            std::string r(s);
            r.erase(
                std::find_if(r.rbegin(), r.rend(),
                             [](unsigned char c) { return !std::isspace(c); })
                    .base(),
                r.end());
            return r;
        }

        std::vector<std::string> split(std::string &s,
                                       const std::string &delim = " ") {
            std::vector<std::string> parts;
            std::size_t cur, prev = 0;
            cur = s.find(delim);
            int i = 0;
            while (cur != std::string::npos && i < 100) {
                i++;
                parts.push_back(s.substr(prev, cur - prev));
                prev = cur + delim.length();
                cur = s.find(delim, prev);
            }
            parts.push_back(s.substr(prev, cur - prev));
            return parts;
        }

    } // namespace string

} // namespace util
