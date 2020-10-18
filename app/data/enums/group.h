#ifndef GROUP_H
#define GROUP_H

#include <string>

enum class Group {
    UNDEFINED,
    STANDARD,
    DOUBLE,
    ONION,
    P2P,
    OBFUSCATED,
};

std::string group2string(Group g);

std::string group2connectName(Group g);

Group groupFromInt(int32_t groupId);

#endif // GROUP_H
