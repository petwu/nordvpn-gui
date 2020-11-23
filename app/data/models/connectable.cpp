#include "connectable.h"

bool Connectable::fuzzyMatchNames(const std::string &n1,
                                  const std::string &n2) {
    std::string _n1 = util::string::toLower(n1);
    _n1 = util::string::replaceAll(n1, "_", "");
    _n1 = util::string::replaceAll(n1, " ", "");
    std::string _n2 = util::string::toLower(n2);
    _n2 = util::string::replaceAll(n1, "_", "");
    _n2 = util::string::replaceAll(n1, " ", "");
    return _n1 == _n2;
}
