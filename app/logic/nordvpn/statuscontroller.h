#ifndef STATUSCONTROLLER_H
#define STATUSCONTROLLER_H

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

struct ConnectionStatus {
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
};

class StatusController : public BaseController {
  public:
    bool canExecuteShellCmds();
    bool isNordVpnInstalled();
    std::string getVersion();
    std::unique_ptr<ConnectionStatus> getStatus();
    uint8_t getRatingMin();
    uint8_t getRatingMax();
    void rate(uint8_t rating);
};

#endif // STATUSCONTROLLER_H
