#include "country.h"

std::string Country::toJSON() {
    json j = json::parse(Location::toJSON());
    j["connectName"] = this->connectName;
    j["countryCode"] = this->countryCode;
    j["offsetLeft"] = this->offsetLeft;
    j["offsetTop"] = this->offsetTop;
    for (auto c : this->cities)
        j["cities"].push_back(json::parse(c.toJSON()));
    return j.dump();
}

Country Country::fromJSON(const std::string &s) {
    Country country;
    json j = json::parse(s);
    if (j["id"].is_number_integer())
        country.id = json::number_integer_t(j["id"]);
    if (j["name"].is_string())
        country.name = json::string_t(j["name"]);
    if (j["lat"].is_number_float())
        country.lat = json::number_float_t(j["lat"]);
    if (j["lng"].is_number_float())
        country.lng = json::number_float_t(j["lng"]);
    if (j["connectName"].is_string())
        country.connectName = json::string_t(j["connectName"]);
    if (j["countryCode"].is_string())
        country.countryCode = json::string_t(j["countryCode"]);
    if (j["offsetLeft"].is_number_float())
        country.offsetLeft = json::number_float_t(j["offsetLeft"]);
    if (j["offsetTop"].is_number_float())
        country.offsetTop = json::number_float_t(j["offsetTop"]);
    if (j["cities"].is_array())
        for (auto c : j["cities"])
            if (c.is_object())
                country.cities.push_back(Location::fromJSON(c.dump()));
    return std::move(country);
}
