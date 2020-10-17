#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <array>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "common/io/asyncprocess.h"
#include "common/io/process.h"
#include "common/util/strings.h"
#include "config.h"

class BaseController {
  public:
    BaseController() {}
};

#endif // BASECONTROLLER_H
