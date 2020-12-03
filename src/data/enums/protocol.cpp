#include "protocol.h"

auto protocolToString(const Nullable<Protocol> &p) -> std::string {
    if (p.isNull()) {
        return "null";
    }
    return protocolToString(p.value());
}

auto protocolToString(const Protocol p) -> std::string {
    switch (p) {
    case Protocol::TCP:
        return "TCP";
    case Protocol::UDP:
        return "UDP";
    default:
        return "";
    }
}

auto protocolFromString(const std::string &s) -> Protocol {
    if (s == "TCP") {
        return Protocol::TCP;
    }
    if (s == "UDP") {
        return Protocol::UDP;
    }
    { return Protocol::Undefined; }
}

auto operator|(Protocol lhs, Protocol rhs) -> Protocol {
    return static_cast<Protocol>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

auto operator&(Protocol lhs, Protocol rhs) -> Protocol {
    return static_cast<Protocol>(static_cast<int>(lhs) & static_cast<int>(rhs));
}
