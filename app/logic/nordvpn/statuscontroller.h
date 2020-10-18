#ifndef STATUSCONTROLLER_H
#define STATUSCONTROLLER_H

#include <atomic>
#include <functional>
#include <thread>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "basecontroller.h"
#include "logic/models/connectioninfo.h"
#include "servercontroller.h"

class IConnectionInfoSubscription {
  public:
    virtual void updateConnectionInfo(const ConnectionInfo &newInfo) = 0;
};

// Singleton:
// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class StatusController : public BaseController {
  public:
    StatusController(const StatusController &) = delete;
    void operator=(const StatusController &) = delete;
    static StatusController &getInstance();

    bool canExecuteShellCmds();
    bool isNordVpnInstalled();
    std::string getVersion();

    ConnectionInfo getStatus();
    void startBackgroundTask();
    void stopBackgroundTask();
    void attach(IConnectionInfoSubscription *subscriber);
    void detach(IConnectionInfoSubscription *subscriber);

    uint8_t getRatingMin();
    uint8_t getRatingMax();
    void rate(uint8_t rating);

  private:
    StatusController();

    std::atomic<bool> _performBackgroundTask = false;
    std::vector<IConnectionInfoSubscription *> _subscribers;
    ConnectionInfo _currentInfo;
    ConnectionInfo _currentConnectedInfo;
    void _backgroundTask();
    void _notifySubscribers();
    std::vector<Country> _countries;
    int32_t _getServerNr(std::string server);
};

#endif // STATUSCONTROLLER_H
