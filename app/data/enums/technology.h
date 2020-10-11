#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include <string>

enum class Technology {
    Undefined,
    OpenVPN,
    NordLynx,
};

std::string technologyToString(const Technology t);

Technology technologyFromString(const std::string &s);

#endif // TECHNOLOGY_H
