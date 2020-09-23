#ifndef BASEREPOSITORY_H
#define BASEREPOSITORY_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "common/util/strings.h"
#include "config.h"

class BaseRepository {
  protected:
    static std::string readFile(std::string path);
};

#endif // BASEREPOSITORY_H
