#ifndef STATUSCONTROLLER_H
#define STATUSCONTROLLER_H

#include <atomic>
#include <functional>
#include <thread>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "basecontroller.h"
#include "servercontroller.h"

class ConnectionStatus {
  public:
    enum Value : int { Disconnected, Connecting, Connected };
    ConnectionStatus() = default;
    constexpr ConnectionStatus(Value v) : value(v) {}
    operator Value() const { return value; }
    std::string toString() const {
        switch (value) {
        case Disconnected:
            return "disconnected";
        case Connecting:
            return "connecting";
        case Connected:
            return "connected";
        default:
            return "";
        }
    }

  private:
    Value value;
};

class ConnectionType {
  public:
    enum Value : int { Undefined, TCP, UDP };
    ConnectionType() = default;
    constexpr ConnectionType(Value v) : value(v) {}
    operator Value() const { return value; }
    std::string toString() const {
        switch (value) {
        case TCP:
            return "TCP";
        case UDP:
            return "UDP";
        default:
            return "";
        }
    }
    static ConnectionType fromString(std::string s) {
        if (s == "TCP")
            return TCP;
        if (s == "UDP")
            return UDP;
        else
            return Undefined;
    }

  private:
    Value value;
};

class Technology {
  public:
    enum Value : int { Undefined, OpenVPN, NordLynx };
    Technology() = default;
    constexpr Technology(Value v) : value(v) {}
    operator Value() const { return value; }
    std::string toString() const {
        if (value == OpenVPN)
            return "OpenVPN";
        if (value == NordLynx)
            return "NordLynx";
        else
            return "";
    }
    static Technology fromString(std::string s) {
        if (s == "OpenVPN")
            return OpenVPN;
        if (s == "NordLynx")
            return NordLynx;
        else
            return Undefined;
    }

  private:
    Value value;
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
    ConnectionType connectionType = ConnectionType::Undefined;
    uint64_t sent = 0;
    uint64_t received = 0;
    uint64_t uptime = 0;
    std::string toString(bool onLine = true) const;
};

class IConnectionInfoSubscription {
  public:
    virtual void update(const ConnectionInfo &newInfo) = 0;
};

class StatusController : public BaseController {
  public:
    StatusController();

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
    std::atomic<bool> _performBackgroundTask = false;
    std::vector<IConnectionInfoSubscription *> _subscribers;
    ConnectionInfo _currectStatus;
    void _backgroundTask();
    void _notifySubscribers();
    json::array_t _countries;
    int32_t _getCountryId(std::string name);
    int32_t _getServerId(std::string server);
};

#endif // STATUSCONTROLLER_H
