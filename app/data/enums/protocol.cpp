#include "protocol.h"

std::string protocolToString(const Protocol ct) {
    switch (ct) {
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
