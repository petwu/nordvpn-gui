#include "technology.h"

std::string technologyToString(const Technology t) {
    switch (t) {
    case Technology::OpenVPN:
        return "OpenVPN";
    case Technology::NordLynx:
        return "NordLynx";
    default:
        return "";
    }
}

Technology technologyFromString(const std::string &s) {
    if (s == "OpenVPN")
        return Technology::OpenVPN;
    if (s == "NordLynx")
        return Technology::NordLynx;
    else
        return Technology::Undefined;
}
