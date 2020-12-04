#ifndef CONNECTABLE_H
#define CONNECTABLE_H

#include <string>

/**
 * @brief The Connectable class is a base class for all classes that are hold
 * information that can be used to connect to a server.
 */
class Connectable {
  public:
    /**
     * @brief connectName is a string identifier that can be passed to the
     * NordVPN CLI to connect to a server. The name can e.g. be a country name
     * or a concrete server identifier. The name must be interpretable by the
     * `nordvpn connect` command.
     */
    std::string connectName;

    /**
     * @brief Check if two connect strings fuzzy match. The are compared all
     * lowercase and underscores are treated the same as spaces.
     * @param n1
     * @param n2
     * @return True, if `n1` and `n2` match complete or at least almost under
     * the given conditions.
     */
    static auto fuzzyMatchNames(const std::string &n1, const std::string &n2)
        -> bool;
};

#endif // CONNECTABLE_H
