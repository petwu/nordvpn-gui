#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include "basecontroller.h"
#include "data/repositories/preferencesrepository.h"
#include "data/repositories/serverrepository.h"

class IRecentCountriesSubscription {
  public:
    virtual void updateRecents(const std::vector<Country> &newRecents) = 0;
};

// Singleton:
// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class ServerController : public BaseController {
  public:
    ServerController(const ServerController &) = delete;
    void operator=(const ServerController &) = delete;
    static ServerController &getInstance();
    std::vector<Country> getAllCountries();
    std::vector<Server> getServersByCountry(int32_t countryId);
    std::vector<Server> getServersByCity(int32_t cityId);
    std::vector<Country> getRecentCountries();
    void removeFromRecentsList(uint32_t countryId);
    void quickConnect();
    void connectToCountryById(uint32_t id);
    void connectToServerById(uint32_t id);
    void disconnect();
    void attach(IRecentCountriesSubscription *subscriber);
    void detach(IRecentCountriesSubscription *subscriber);

  private:
    ServerController();
    std::vector<Country> _allCountries;
    std::vector<Country> _recents;
    std::vector<Server> _allServers;
    std::vector<IRecentCountriesSubscription *> _subscribers;
    void _notifySubscribers();
};

#endif // SERVERCONTROLLER_H
