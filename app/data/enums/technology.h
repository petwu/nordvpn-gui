#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include <string>

/**
 * @brief The Technology enum identifies the main technology categories NordVPN
 * divides its servers into.
 */
enum class Technology {
    Undefined,
    OpenVPN,
    NordLynx,
};

/**
 * @brief Convert a Technology value into a human readable string.
 */
std::string technologyToString(const Technology t);

/**
 * @brief Inverse operation of #technologyToString.
 */
Technology technologyFromString(const std::string &s);

#endif // TECHNOLOGY_H
