#ifndef CONNECTABLE_H
#define CONNECTABLE_H

#include <string>

#include "common/util/strings.h"

class Connectable {
  public:
    std::string connectName = "";

    static bool fuzzyMatchNames(const std::string &n1, const std::string &n2);
};

#endif // CONNECTABLE_H
