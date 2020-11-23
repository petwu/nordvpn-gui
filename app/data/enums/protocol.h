#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

#include "common/types/nullable.h"

/**
 * @brief The Protocol enum identifies a network protocol. The values are are
 * bit flags, meaning only one unique bit is 1 for each value. The enum value
 * can therefore be manipulated with bitwise operators like & or |.
 */
enum class Protocol {
    Undefined = 0, ///< 0b0000
    UDP = 1 << 1,  ///< 0b0001
    TCP = 1 << 2,  ///< 0b0010
};

/**
 * @brief Convert a Protocol to a human readable string.
 * @param p A nullable Protocol value.
 * @return "null" literal in case `p` is null.
 */
std::string protocolToString(const Nullable<Protocol> p);

/**
 * @brief Convert a Protocol to a human readable string.
 */
std::string protocolToString(const Protocol p);

/**
 * @brief Inverse operation of `protocolToString`.
 */
Protocol protocolFromString(const std::string &s);

/**
 * @brief Perform a bitwise OR operation.
 * @param lhs Left hand side of the operator.
 * @param rhs Right hand side of the operator.
 * @return A Protocol value representing both Protocol flags. Must not be an
 * actual Protocol enum value.
 */
Protocol operator|(Protocol lhs, Protocol rhs);

/**
 * @brief Perform a bitwise AND operation.
 * @param lhs Left hand side of the operator.
 * @param rhs Right hand side of the operator.
 * @return Protocol::Undefined in case `lhs` and `rhs` don't have any flags in
 * common.
 */
Protocol operator&(Protocol lhs, Protocol rhs);

#endif // PROTOCOL_H
