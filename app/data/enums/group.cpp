#include "group.h"

std::string group2string(Group g) {
    switch (g) {
    case Group::Standard:
        return "Standard";
    case Group::Double:
        return "Double";
    case Group::Onion:
        return "Onion";
    case Group::P2P:
        return "P2P";
    case Group::Obfuscated:
        return "Obfuscated";
    default:
        return "";
    }
}

std::string group2connectName(Group g) {
    switch (g) {
    case Group::Standard:
        return "Standard_VPN_Servers";
    case Group::Double:
        return "Double_VPN";
    case Group::Onion:
        return "Onion_Over_VPN";
    case Group::P2P:
        return "P2P";
    case Group::Obfuscated:
        return "Obfuscated_Servers";
    default:
        return "";
    }
}

Group groupFromInt(int32_t groupId) {
    switch (groupId) {
    case static_cast<int32_t>(Group::Standard):
        return Group::Standard;
    case static_cast<int32_t>(Group::Double):
        return Group::Double;
    case static_cast<int32_t>(Group::Onion):
        return Group::Onion;
    case static_cast<int32_t>(Group::P2P):
        return Group::P2P;
    case static_cast<int32_t>(Group::Obfuscated):
        return Group::Obfuscated;
    default:
        return Group::Undefined;
    }
}
