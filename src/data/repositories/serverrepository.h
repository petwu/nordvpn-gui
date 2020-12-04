#ifndef SERVERREPOSITORY_H
#define SERVERREPOSITORY_H

#include <vector>

#include "data/models/country.h"
#include "data/models/server.h"
#include "data/repositories/baserepository.h"

/**
 * @brief The ServerRepository class prvides static functions to retrieve and
 * cache the list of available servers and countries.
 */
class ServerRepository : public BaseRepository {
  public:
    /**
     * @brief Fetch the list of servers from the NordVPN API.
     * @details After the list was retrieved it will be cached in a file. The
     * cached list can be retrieved with #fetchServersFromCache().
     * @return A list of servers. The list will be empty, if something went
     * wrong (connection timeout, error HTTP code, etc.).
     */
    static auto fetchServers() -> std::vector<Server>;

    /**
     * @brief Fetch the cached list of servers.
     * @return A list of servers. An empty list, in case no cached server list
     * exists. This is only the case, if #fetchServers() was not called before
     * (successfully).
     */
    static auto fetchServersFromCache() -> std::vector<Server>;

    /**
     * @brief Fetch the list of countries from the NordVPN API.
     * @details After the list was retrieved it will be cached in a file. The
     * cached list can be retrieved with #fetchCountriesFromCache().
     * @return A list of countries. The list will be empty, if something went
     * wrong (connection timeout, error HTTP code, etc.).
     */
    static auto fetchCountries() -> std::vector<Country>;

    /**
     * @brief Fetch the cached list of countries.
     * @return A list of countries. An empty list, in case no cached server list
     * exists. This is only the case, if #fetchCountries() was not called before
     * (successfully).
     */
    static auto fetchCountriesFromCache() -> std::vector<Country>;

  private:
    /**
     * @brief Save the list of servers to a file.
     * @details This is used to cache the list, since a file can be faster
     * retrieved than the response of a network request. The cached list can be
     * retrieved with #fetchServersFromCache().
     * @param serverList The list of server to save to a file.
     */
    static void saveServerListToFile(std::vector<Server> &serverList);

    /**
     * @brief Save the list of countries to a file.
     * @details This is used to cache the list, since a file can be faster
     * retrieved than the response of a network request. The cached list can be
     * retrieved with #fetchCountriesFromCache().
     * @param countryList The list of countries to save to a file.
     */
    static void saveCountryListToFile(std::vector<Country> &countryList);
};

#endif // SERVERREPOSITORY_H
