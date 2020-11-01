#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

#include "common/types/nullable.h"

enum class Protocol {
    Undefined = 0, /// 0b0000
    UDP = 1 << 0,  /// 0b0001
    TCP = 1 << 1,  /// 0b0010
};

std::string protocolToString(const Nullable<Protocol> p);

std::string protocolToString(const Protocol p);

Protocol protocolFromString(const std::string &s);

Protocol operator|(Protocol lhs, Protocol rhs);
Protocol operator&(Protocol lhs, Protocol rhs);

#endif // PROTOCOL_H
