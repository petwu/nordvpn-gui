#ifndef BASEREPOSITORY_H
#define BASEREPOSITORY_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "curl/curl.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "common/util/strings.h"
#include "config.h"
#include "data/enums/group.h"
#include "data/enums/protocol.h"
#include "data/enums/securityprotocol.h"
#include "data/enums/technology.h"

/**
 * Number of seconds before a libcurl call (e.g. http request) times out and
 * gets aborted.
 */
constexpr int DEFAULT_CURL_TIMEOUT = 30; // seconds

/**
 * @brief The BaseRepository class is a base class for all repositories that
 * provides common utility functions, e.g. for filesystem or network access.
 */
class BaseRepository {
  protected:
    /**
     * @brief Read in a file.
     * @param path Path to the file to read.
     * @return The contents of the file or an empty string if the file does not
     * exist.
     */
    static auto readFile(const std::filesystem::path &path) -> std::string;

    /**
     * @brief Write a text file. Existing files get overridden.
     * @param path The path to the file.
     * @param content The text that should be written to the file.
     */
    static void writeFile(const std::filesystem::path &path,
                          const std::string &content);

    /**
     * @brief Perform a HTTP network request with the help of libcurl.
     * @param url HTTP(S) URL of the network resource to access.
     * @param timeoutSec Max. number of seconds the request may take to finish.
     * @return The body of the HTTP response in case the server responded with
     * code 200. An empty string in case of any other HTTP code or the requested
     * timed out.
     */
    static auto curl(const char *&url,
                     uint8_t timeoutSec = DEFAULT_CURL_TIMEOUT) -> std::string;

    /**
     * @brief Overload of #curl() with `std::string` instead of `char*`.
     */
    static auto curl(const std::string &url,
                     uint8_t timeoutSec = DEFAULT_CURL_TIMEOUT) -> std::string;
};

#endif // BASEREPOSITORY_H
