#ifndef SERVERREPOSITORY_HPP
#define SERVERREPOSITORY_HPP

#include <vector>

#include "baserepository.hpp"
#include "data/models/country.hpp"
#include "data/models/server.hpp"
#include "iserverrepository.hpp"

class ServerRepository : public IServerRepository, public BaseRepository {
  public:
    auto fetchServers() -> std::vector<Server> override;
    auto fetchServersFromCache() -> std::vector<Server> override;
    auto fetchCountries() -> std::vector<Country> override;
    auto fetchCountriesFromCache() -> std::vector<Country> override;

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

#endif // SERVERREPOSITORY_HPP
