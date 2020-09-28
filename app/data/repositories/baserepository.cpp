#include "baserepository.h"

std::string BaseRepository::readFile(std::filesystem::path path) {
    std::ifstream file(path);
    std::stringstream buf;
    buf << file.rdbuf();
    file.close();
    return buf.str();
}

void BaseRepository::writeFile(std::filesystem::path path,
                               std::string content) {
    std::filesystem::create_directories(path.parent_path());
    std::fstream file;
    file.open(path, std::fstream::out | std::fstream::trunc);
    file << content << std::endl;
    file.close();
}

size_t curlCallback(const char *in, size_t size, size_t num, std::string *out) {
    size_t totalBytes = size * num;
    out->append(in, totalBytes);
    return totalBytes;
}

std::string BaseRepository::curl(std::string url, uint8_t timeoutSec) {
    long httpCode = 0;
    std::string httpData;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSec);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpData);
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);
    if (httpCode == 200) {
        return std::move(httpData);
    }
    return "";
}
