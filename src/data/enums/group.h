#ifndef GROUP_H
#define GROUP_H

#include <cstdint>
#include <string>

/**
 * @brief The Group enum identifies a server group.
 */
enum class Group {
    Undefined,
    Standard,
    Double,
    Onion,
    P2P,
    Obfuscated,
};

/**
 * @brief Convert a Group enum to a string.
 */
auto group2string(Group g) -> std::string;

/**
 * @brief Convert a Group enum to a string that can be used as connect parameter
 * to the NordVPN CLI.
 */
auto group2connectName(Group g) -> std::string;

/**
 * @brief Convert an integer to its corresponding Group enum value.
 * @return The corresponding Group value. Defaults to Group::UNDEFINED for
 * undefined cases.
 */
auto groupFromInt(int32_t groupId) -> Group;

#endif // GROUP_H
