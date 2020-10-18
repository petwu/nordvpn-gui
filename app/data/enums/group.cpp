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

std::string group2connectName(Group g) {
    switch (g) {
    case Group::STANDARD:
        return "Standard_VPN_Servers";
    case Group::DOUBLE:
        return "Double_VPN";
    case Group::ONION:
        return "Onion_Over_VPN";
    case Group::P2P:
        return "P2P";
    case Group::OBFUSCATED:
        return "Obfuscated_Servers";
    default:
        return "";
    }
}

Group groupFromInt(int32_t groupId) {
    switch (groupId) {
    case static_cast<int32_t>(Group::STANDARD):
        return Group::STANDARD;
    case static_cast<int32_t>(Group::DOUBLE):
        return Group::DOUBLE;
    case static_cast<int32_t>(Group::ONION):
        return Group::ONION;
    case static_cast<int32_t>(Group::P2P):
        return Group::P2P;
    case static_cast<int32_t>(Group::OBFUSCATED):
        return Group::OBFUSCATED;
    default:
        return Group::UNDEFINED;
    }
}
