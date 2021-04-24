#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

#include "nordvpn/models/connectable.hpp"

/**
 * @brief The Location class is a minimal representation of a location on a map.
 * It extends Connectable since every location has to contain server you can
 * connect to.
 */
class Location : public Connectable {
  public:
    /**
     * @brief Every location is identified by a unique ID.
     */
    int32_t id = -1;

    /**
     * @brief The locations name, e.g. the country, region or city name.
     */
    std::string name;

    /**
     * @brief latitude
     */
    float lat = 0;

    /**
     * @brief longitude
     */
    float lng = 0;

    /**
     * @brief Convert the object into a JSON string representation.
     * @return
     */
    auto toJSON() -> std::string;

    /**
     * @brief Inverse operation of #toJSON.
     * @param s
     * @return
     */
    static auto fromJSON(const std::string &s) -> Location;
};

#endif // LOCATION_HPP
