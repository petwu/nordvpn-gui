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

/**
 * @brief The BaseController class provides common functionality for all
 * controller classes in this directory.
 *
 * @note Currenly, this class does not provide any function. Nevertheless it is
 * not discarded and every controller should extend it in case it is required in
 * future.
 */
class BaseController {};

#endif // BASECONTROLLER_H
