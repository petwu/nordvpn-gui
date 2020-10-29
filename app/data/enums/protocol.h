#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

#include "common/types/nullable.h"

enum class Protocol {
    Undefined,
    TCP,
    UDP,
};

std::string protocolToString(const Nullable<Protocol> p);

std::string protocolToString(const Protocol p);

Protocol protocolFromString(const std::string &s);

#endif // PROTOCOL_H
