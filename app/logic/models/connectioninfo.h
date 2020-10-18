#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <string>
#include <vector>

#include "data/enums/group.h"
#include "data/enums/protocol.h"
#include "data/enums/technology.h"

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
    int32_t serverNr = 0;
    std::string country = "";
    int32_t countryId = -1;
    std::string city = "";
    std::string ip = "";
    Technology technology = Technology::Undefined;
    Protocol connectionType = Protocol::Undefined;
    uint64_t sent = 0;
    uint64_t received = 0;
    uint64_t uptime = 0;

    std::vector<Group> groups;
    uint8_t load;

    bool isEmpty() const;
};

#endif // CONNECTIONINFO_H
