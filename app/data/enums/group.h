#ifndef GROUP_H
#define GROUP_H

#include <string>

enum class Group {
    Undefined,
    Standard,
    Double,
    Onion,
    P2P,
    Obfuscated,
};

std::string group2string(Group g);

std::string group2connectName(Group g);

Group groupFromInt(int32_t groupId);

#endif // GROUP_H
