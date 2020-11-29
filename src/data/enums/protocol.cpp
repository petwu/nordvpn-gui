#include "protocol.h"

std::string protocolToString(const Nullable<Protocol> p) {
    if (p.isNull()) {
        return "null";
    }
    return protocolToString(p.value());
}

std::string protocolToString(const Protocol p) {
    switch (p) {
    case Protocol::TCP:
        return "TCP";
    case Protocol::UDP:
        return "UDP";
    default:
        return "";
    }
}

Protocol protocolFromString(const std::string &s) {
    if (s == "TCP")
        return Protocol::TCP;
    if (s == "UDP")
        return Protocol::UDP;
    else
        return Protocol::Undefined;
}

Protocol operator|(Protocol lhs, Protocol rhs) {
    return static_cast<Protocol>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

Protocol operator&(Protocol lhs, Protocol rhs) {
    return static_cast<Protocol>(static_cast<int>(lhs) & static_cast<int>(rhs));
}