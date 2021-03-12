#include "connectable.h"

#include "common/util/strings.h"

auto Connectable::fuzzyMatchNames(const std::string &n1, const std::string &n2)
    -> bool {
    std::string _n1 = util::string::toLower(n1);
    _n1 = util::string::replaceAll(n1, "_", "");
    _n1 = util::string::replaceAll(_n1, " ", "");
    std::string _n2 = util::string::toLower(n2);
    _n2 = util::string::replaceAll(n2, "_", "");
    _n2 = util::string::replaceAll(_n2, " ", "");
    return _n1 == _n2;
}
