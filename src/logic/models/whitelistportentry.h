#ifndef WHITELISTPORTENTRY_H
#define WHITELISTPORTENTRY_H

#include "data/enums/protocol.h"

/**
 * @brief The WhitelistPortEntry class represents an entry in the ports
 * whitelist. A port number is a 16 bit unsigned integer and thus ranging from 0
 * to 65535. Hence uint16_t would be enough, but choosing uint32_t as a data
 * type allows to represent invalid ports (provided through user input).
 */
class WhitelistPortEntry {
  public:
    /**
     * @brief Start of the port range.
     */
    uint32_t portFrom = 0;

    /**
     * @brief End of the port range.
     */
    uint32_t portTo = 0;

    /**
     * @brief Flag that inicates the used protocols. Use the binary or operator
     * to set multple protocols, e.g. Protocol::TCP | Protocol::UDP .
     */
    Protocol protocolFlag = Protocol::TCP | Protocol::UDP;

    /**
     * @brief operator == for a member-by-member equality check.
     * @param other
     * @return True, if every member variables of the two compared objects
     * match, false otherwise.
     */
    auto operator==(const WhitelistPortEntry &other) const -> bool;

    /**
     * @brief operator < is required in order to be used as a std::set<> key.
     * @param other The other object to compare with.
     * @return True, if considered to be smaller. False otherwise.
     */
    auto operator<(const WhitelistPortEntry &other) const -> bool;
};

#endif // WHITELISTPORTENTRY_H
