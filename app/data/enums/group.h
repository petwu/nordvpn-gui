#ifndef GROUP_H
#define GROUP_H

#include <string>

enum class Group {
    STANDARD,
    DOUBLE,
    ONION,
    P2P,
    OBFUSCATED,
};

std::string group2string(Group g);

#endif // GROUP_H
