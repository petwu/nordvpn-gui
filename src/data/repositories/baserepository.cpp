#include "baserepository.hpp"

#include <cstddef>
#include <curl/curl.h>
#include <fstream>
#include <type_traits>
#include <utility>

auto BaseRepository::readFile(const std::filesystem::path &path)
    -> std::string {
    std::ifstream file(path);
    std::stringstream buf;
    buf << file.rdbuf();
    file.close();
    return buf.str();
}

void BaseRepository::writeFile(const std::filesystem::path &path,
                               const std::string &content) {
    std::filesystem::create_directories(path.parent_path());
    std::fstream file;
    file.open(path, std::fstream::out | std::fstream::trunc);
    file << content << std::endl;
    file.close();
}

auto curlCallback(const char *in, size_t size, size_t num, std::string *out)
    -> size_t {
    size_t totalBytes = size * num;
    out->append(in, totalBytes);
    return totalBytes;
}

auto BaseRepository::curl(const char *&url, const uint8_t timeoutSec,
                          const std::vector<std::string> &headers)
    -> std::string {
    return BaseRepository::curl(std::string(url), timeoutSec, headers);
}

auto BaseRepository::curl(const std::string &url, const uint8_t timeoutSec,
                          const std::vector<std::string> &headers)
    -> std::string {
    std::string httpData;
    CURL *curl = curl_easy_init();
    // curl_easy_setopt cannot be used without varargs
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSec);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpData);
    struct curl_slist *headerList = nullptr;
    if (!headers.empty()) {
        for (const auto &header : headers) {
            headerList = curl_slist_append(headerList, header.c_str());
        }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }
    CURLcode curlCode = curl_easy_perform(curl);
    curl_slist_free_all(headerList);
    curl_easy_cleanup(curl);
    return curlCode == CURLE_OK ? std::move(httpData) : "";
}
