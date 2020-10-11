#include "connectable.h"

bool Connectable::fuzzyMatchNames(const std::string &n1,
                                  const std::string &n2) {
    return util::string::toLower(util::string::replaceAll(n1, "_", "")) ==
           util::string::toLower(util::string::replaceAll(n2, "_", ""));
}
