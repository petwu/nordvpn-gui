#ifndef STATUSCONTROLLER_H
#define STATUSCONTROLLER_H

#include <atomic>
#include <functional>
#include <thread>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "basecontroller.h"
#include "servercontroller.h"

enum class ConnectionStatus {
    Disconnected,
    Connecting,
    Connected,
};

class ConnectionInfo {
  public:
    ConnectionInfo() = default;
    ConnectionInfo(const ConnectionInfo &c) = default;
    ConnectionInfo &operator=(const ConnectionInfo &) = default;
    ConnectionInfo(ConnectionInfo &&m) = default;
    ConnectionInfo &operator=(ConnectionInfo &&) = default;

    ConnectionStatus status = ConnectionStatus::Disconnected;
    std::string server = "";
    int32_t serverId = 0;
    std::string country = "";
    int32_t countryId = -1;
    std::string city = "";
    std::string ip = "";
    Technology technology = Technology::Undefined;
    Protocol connectionType = Protocol::Undefined;
    uint64_t sent = 0;
    uint64_t received = 0;
    uint64_t uptime = 0;

    bool isEmpty() const;
};

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
    int32_t _getCountryId(std::string name);
    int32_t _getServerId(std::string server);
};

#endif // STATUSCONTROLLER_H
