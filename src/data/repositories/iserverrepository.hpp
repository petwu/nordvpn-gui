#ifndef ISERVERREPOSITORY_HPP
#define ISERVERREPOSITORY_HPP

#include <vector>

/**
 * @brief The IServerRepository interface prvides static functions to retrieve
 * and cache the list of available servers and countries.
 */
class IServerRepository {
  public:
    /**
     * @brief Fetch the list of servers from the NordVPN API.
     * @details After the list was retrieved it will be cached in a file. The
     * cached list can be retrieved with #fetchServersFromCache().
     * @return A list of servers. The list will be empty, if something went
     * wrong (connection timeout, error HTTP code, etc.).
     */
    virtual auto fetchServers() -> std::vector<Server> = 0;

    /**
     * @brief Fetch the cached list of servers.
     * @return A list of servers. An empty list, in case no cached server list
     * exists. This is only the case, if #fetchServers() was not called before
     * (successfully).
     */
    virtual auto fetchServersFromCache() -> std::vector<Server> = 0;

    /**
     * @brief Fetch the list of countries from the NordVPN API.
     * @details After the list was retrieved it will be cached in a file. The
     * cached list can be retrieved with #fetchCountriesFromCache().
     * @return A list of countries. The list will be empty, if something went
     * wrong (connection timeout, error HTTP code, etc.).
     */
    virtual auto fetchCountries() -> std::vector<Country> = 0;

    /**
     * @brief Fetch the cached list of countries.
     * @return A list of countries. An empty list, in case no cached server list
     * exists. This is only the case, if #fetchCountries() was not called before
     * (successfully).
     */
    virtual auto fetchCountriesFromCache() -> std::vector<Country> = 0;
};

#endif // ISERVERREPOSITORY_HPP
