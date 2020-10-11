#include "group.h"

std::string group2string(Group g) {
    switch (g) {
    case Group::STANDARD:
        return "Standard";
    case Group::DOUBLE:
        return "Double";
    case Group::ONION:
        return "Onion";
    case Group::P2P:
        return "P2P";
    case Group::OBFUSCATED:
        return "Obfuscated";
    default:
        return "";
    }
}
