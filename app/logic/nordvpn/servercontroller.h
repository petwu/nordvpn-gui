#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include "basecontroller.h"
#include "data/repositories/preferencesrepository.h"
#include "data/repositories/serverrepository.h"

class IRecentCountriesSubscription {
  public:
    virtual void updateRecents(const std::vector<Country> &newRecents) = 0;
};

class ServerController : public BaseController {
  public:
    ServerController();
    std::vector<Country> getAllCountries();
    std::vector<std::string> getCities(std::string country);
    std::vector<Server> getServersByCountry(int32_t countryId);
    std::vector<Server> getServersByCity(int32_t cityId);
    std::vector<std::string> getGroups();
    std::vector<Country> getRecentCountries();
    void removeFromRecentsList(uint32_t countryId);
    void quickConnect();
    void connectToCountryById(uint32_t id);
    void connectToServerById(uint32_t id);
    void disconnect();
    void attach(IRecentCountriesSubscription *subscriber);
    void detach(IRecentCountriesSubscription *subscriber);

  private:
    std::vector<Country> _allCountries;
    std::vector<Country> _recents;
    std::vector<Server> _allServers;
    std::vector<IRecentCountriesSubscription *> _subscribers;
    void _notifySubscribers();
};

#endif // SERVERCONTROLLER_H
