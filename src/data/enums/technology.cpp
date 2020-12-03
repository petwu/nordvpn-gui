#include "technology.h"

auto technologyToString(const Technology t) -> std::string {
    switch (t) {
    case Technology::OpenVPN:
        return "OpenVPN";
    case Technology::NordLynx:
        return "NordLynx";
    default:
        return "";
    }
}

auto technologyFromString(const std::string &s) -> Technology {
    if (s == "OpenVPN") {
        return Technology::OpenVPN;
    }
    if (s == "NordLynx") {
        return Technology::NordLynx;
    }
    { return Technology::Undefined; }
}
