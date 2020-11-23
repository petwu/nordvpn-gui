#ifndef GROUP_H
#define GROUP_H

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
std::string group2string(Group g);

/**
 * @brief Convert a Group enum to a string that can be used as connect parameter
 * to the NordVPN CLI.
 */
std::string group2connectName(Group g);

/**
 * @brief Convert an integer to its corresponding Group enum value.
 * @return The corresponding Group value. Defaults to Group::UNDEFINED for
 * undefined cases.
 */
Group groupFromInt(int32_t groupId);

#endif // GROUP_H
