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
    std::vector<Country> getAllCountries();
    std::vector<std::string> getCities(std::string country);
    std::vector<std::string> getGroups();
    std::vector<Country> getRecentCountries();
    void removeFromRecentsList(uint32_t countryId);
    void quickConnect();
    void connectToCountryById(uint32_t id);
    void disconnect();
    void attach(IRecentCountriesSubscription *subscriber);
    void detach(IRecentCountriesSubscription *subscriber);

  private:
    std::vector<Country> _allCountries;
    std::vector<Country> _recents;
    std::vector<IRecentCountriesSubscription *> _subscribers;
    void _notifySubscribers();
};

#endif // SERVERCONTROLLER_H
