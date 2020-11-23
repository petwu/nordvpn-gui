#ifndef COUNTRY_H
#define COUNTRY_H

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "connectable.h"
#include "location.h"

/**
 * @brief The Country class represents a country with NordVPN servers. It
 * extends Connectable since you can connect to a server by specifying only the
 * country.
 */
class Country : public Location, public Connectable {
  public:
    /**
     * @brief ISO 3166-1 Alpha-2 country code.
     */
    std::string countryCode;

    /**
     * @brief Offsert from the left side of the map. Used to position the
     * marker. Must be a percentage value between 0 and 1.
     */
    float offsetLeft = 0;

    /**
     * @brief Offsert from the top side of the map. Used to position the
     * marker. Must be a percentage value between 0 and 1.
     */
    float offsetTop = 0;

    /**
     * @brief List of cities with VPN servers in this country.
     */
    std::vector<Location> cities;

    /**
     * @brief Convert the object into a JSON string representation.
     * @return
     */
    std::string toJSON();

    /**
     * @brief Inverse operation of #toJSON.
     * @param s
     * @return
     */
    static Country fromJSON(const std::string &s);
};

#endif // COUNTRY_H
