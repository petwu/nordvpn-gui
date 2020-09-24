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

#include "common/util/strings.h"
#include "config.h"

class CmdResult {
  public:
    CmdResult(std::string out, uint32_t rc);
    CmdResult(const CmdResult &c) = default;
    CmdResult &operator=(const CmdResult &) = default;
    CmdResult(CmdResult &&m) = default;
    CmdResult &operator=(CmdResult &&) = default;

    std::string output;
    uint32_t exitCode;

    bool operator==(const CmdResult &other) const;
    bool operator!=(const CmdResult &other) const;

  private:
    CmdResult() = default;
};

static CmdResult ERROR_POPEN_FAILED("error: unable to execute a shell command",
                                    -1);
class BaseController {
  public:
    BaseController() {}

  protected:
    CmdResult execute(std::string cmd);
    void executeNonBlocking(std::string cmd);
};

#endif // BASECONTROLLER_H
