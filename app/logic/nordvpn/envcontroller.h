#ifndef ENVCONTROLLER_H
#define ENVCONTROLLER_H

#include "curl/curl.h"

#include "basecontroller.h"

class EnvInfo {
  public:
    bool internetConnected = false;
    bool nordvpnApiAvailable = false;
    bool shellAvailable = false;
    bool nordvpnInstalled = false;
    bool loggedIn = false;

    std::string toString();
};

class IEnvInfoSubscription {
  public:
    virtual void updateEnv(const EnvInfo &envInfo) = 0;
};

// Singleton:
// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class EnvController : public BaseController {
  public:
    EnvController(const EnvController &) = delete;
    void operator=(const EnvController &) = delete;
    static EnvController &getInstance();

    EnvInfo getEnvInfo();
    void attach(IEnvInfoSubscription *subscriber);
    void detach(IEnvInfoSubscription *subscriber);

  private:
    EnvController() {}
    EnvInfo _envInfo;
    std::vector<IEnvInfoSubscription *> _subscribers;
    void _notifySubscribers();
    bool _isInternetConnected();
    bool _isNordvpnApiAvailable();
    bool _isShellAvailable();
    bool _isNordvpnInstalled();
    bool _isLoggedIn();
};

#endif // ENVCONTROLLER_H
