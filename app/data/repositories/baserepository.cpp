#include "baserepository.h"

std::string BaseRepository::readFile(std::string path) {
    std::filesystem::path fspath;
    for (auto partial : util::string::split(path, "/"))
        fspath /= partial;
    std::ifstream file(fspath);
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}
