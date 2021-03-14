#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <string>

#include "common/types/nullable.hpp"

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
auto protocolToString(const Nullable<Protocol> &p) -> std::string;

/**
 * @brief Convert a Protocol to a human readable string.
 */
auto protocolToString(Protocol p) -> std::string;

/**
 * @brief Inverse operation of `protocolToString`.
 */
auto protocolFromString(const std::string &s) -> Protocol;

/**
 * @brief Perform a bitwise OR operation.
 * @param lhs Left hand side of the operator.
 * @param rhs Right hand side of the operator.
 * @return A Protocol value representing both Protocol flags. Must not be an
 * actual Protocol enum value.
 */
auto operator|(Protocol lhs, Protocol rhs) -> Protocol;

/**
 * @brief Perform a bitwise AND operation.
 * @param lhs Left hand side of the operator.
 * @param rhs Right hand side of the operator.
 * @return Protocol::Undefined in case `lhs` and `rhs` don't have any flags in
 * common.
 */
auto operator&(Protocol lhs, Protocol rhs) -> Protocol;

#endif // PROTOCOL_HPP
