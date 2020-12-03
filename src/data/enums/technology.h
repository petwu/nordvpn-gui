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
auto technologyToString(Technology t) -> std::string;

/**
 * @brief Inverse operation of #technologyToString.
 */
auto technologyFromString(const std::string &s) -> Technology;

#endif // TECHNOLOGY_H
