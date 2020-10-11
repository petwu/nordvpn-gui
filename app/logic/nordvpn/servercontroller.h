#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H

#include <atomic>
#include <thread>

#include "basecontroller.h"
#include "data/repositories/preferencesrepository.h"
#include "data/repositories/serverrepository.h"
#include "settingscontroller.h"

class ICountriesSubscription {
  public:
    virtual void updateRecents(const std::vector<Country> &newRecents) = 0;
    virtual void updateCountryList(const std::vector<Country> &countryList) = 0;
};

// Singleton:
// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class ServerController : public BaseController {
  public:
    ServerController(const ServerController &) = delete;
    void operator=(const ServerController &) = delete;
    static ServerController &getInstance();
    std::vector<Country> getAllCountries(bool updateFromCache = false);
    std::vector<Server> getServersByCountry(int32_t countryId);
    std::vector<Server> getServersByCity(int32_t cityId);
    std::vector<Country> getRecentCountries();
    void removeFromRecentsList(uint32_t countryId);
    void quickConnect();
    void connectToCountryById(uint32_t id);
    void connectToServerById(uint32_t id);
    void disconnect();
    void attach(ICountriesSubscription *subscriber);
    void detach(ICountriesSubscription *subscriber);
    void startBackgroundTask();
    void stopBackgroundTask();

  private:
    ServerController();
    SettingsController &_settingsController = SettingsController::getInstance();
    std::vector<Country> _allCountries;
    std::vector<Country> _recents;
    std::vector<Server> _allServers;
    std::vector<Server> _filterServerList(int32_t countryId, int32_t cityId);
    std::vector<ICountriesSubscription *> _subscribers;
    void _notifySubscribersRecents();
    void _notifySubscribersCountryList();
    std::atomic<bool> _performBackgroundTask = false;
    void _backgroundTaskServerList();
    void _backgroundTaskCountryList();
};

#endif // SERVERCONTROLLER_H
