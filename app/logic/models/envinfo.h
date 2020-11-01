#ifndef ENVINFO_H
#define ENVINFO_H

#include <string>

#include "common/types/nullable.h"

class EnvInfo {
  public:
    bool internetConnected = false;
    bool shellAvailable = false;
    bool nordvpnInstalled = false;
    Nullable<bool> loggedIn;

    std::string toString() const;
};

#endif // ENVINFO_H
