#include "nordvpn/enums/group.hpp"

auto group2string(Group g) -> std::string {
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

auto group2connectName(Group g) -> std::string {
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

auto groupFromInt(int32_t groupId) -> Group {
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
