#ifndef COUNTRY_H
#define COUNTRY_H

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "connectable.h"
#include "location.h"

class Country : public Location, public Connectable {
  public:
    std::string countryCode;
    float offsetLeft = 0;
    float offsetTop = 0;
    std::vector<Location> cities;

    std::string toJSON();
    static Country fromJSON(const std::string &s);
};

#endif // COUNTRY_H
