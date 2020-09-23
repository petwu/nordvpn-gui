#ifndef STATUSCONTROLLER_H
#define STATUSCONTROLLER_H

#include <atomic>
#include <functional>
#include <thread>

#include "basecontroller.h"

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

class ConnectionStatus {
  public:
    ConnectionStatus() = default;
    ConnectionStatus(const ConnectionStatus &c) = default;
    ConnectionStatus &operator=(const ConnectionStatus &) = default;
    ConnectionStatus(ConnectionStatus &&m) = default;
    ConnectionStatus &operator=(ConnectionStatus &&) = default;

    bool connected;
    std::string server;
    std::string country;
    std::string city;
    std::string ip;
    Technology technology;
    ConnectionType connectionType;
    uint64_t sent;
    uint64_t received;
    uint64_t uptime;
    std::string toString(bool onLine = true);
};

class IConnectionStatusSubscription {
  public:
    virtual void update(const ConnectionStatus &newStatus) = 0;
};

class StatusController : public BaseController {
  public:
    bool canExecuteShellCmds();
    bool isNordVpnInstalled();
    std::string getVersion();

    ConnectionStatus getStatus();
    void startBackgroundTask();
    void stopBackgroundTask();
    void attach(std::shared_ptr<IConnectionStatusSubscription> subscriber);
    void detach(std::shared_ptr<IConnectionStatusSubscription> subscriber);

    uint8_t getRatingMin();
    uint8_t getRatingMax();
    void rate(uint8_t rating);

  private:
    std::atomic<bool> _performBackgroundTask = false;
    std::vector<std::shared_ptr<IConnectionStatusSubscription>> _subscribers;
    ConnectionStatus _currectStatus;
    void _backgroundTask();
    void _notifySubscribers();
};

#endif // STATUSCONTROLLER_H
