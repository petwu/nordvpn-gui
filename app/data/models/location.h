#ifndef LOCATION_H
#define LOCATION_H

#include <string>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Location {
  public:
    int32_t id = -1;
    std::string name = "";
    float lat = 0;
    float lng = 0;

    std::string toJSON();
    static Location fromJSON(const std::string &s);
};

#endif // LOCATION_H
