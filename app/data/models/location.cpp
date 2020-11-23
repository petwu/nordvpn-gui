#include "location.h"

std::string Location::toJSON() {
    json j;
    j["connectName"] = this->connectName;
    j["id"] = this->id;
    j["name"] = this->name;
    j["lat"] = this->lat;
    j["lng"] = this->lng;
    return j.dump();
}

Location Location::fromJSON(const std::string &s) {
    Location location;
    json j = json::parse(s);
    if (j["connectName"].is_string())
        location.connectName = json::string_t(j["connectName"]);
    if (j["id"].is_number_integer())
        location.id = json::number_integer_t(j["id"]);
    if (j["name"].is_string())
        location.name = json::string_t(j["name"]);
    if (j["lat"].is_number_float())
        location.lat = json::number_float_t(j["lat"]);
    if (j["lng"].is_number_float())
        location.lng = json::number_float_t(j["lng"]);
    return std::move(location);
}
