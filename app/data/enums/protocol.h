#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

enum class Protocol {
    Undefined,
    TCP,
    UDP,
};

std::string protocolToString(const Protocol ct);

Protocol protocolFromString(const std::string &s);

#endif // PROTOCOL_H
