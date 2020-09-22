#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

#include "common/util/strings.h"
#include "config.h"

struct CmdResult {
    std::string output;
    int exitCode;

    bool operator==(const CmdResult &other) const {
        return output == other.output && exitCode == other.exitCode;
    }
    bool operator!=(const CmdResult &other) const { return !(*this == other); }
};

static CmdResult *ERROR_POPEN_FAILED =
    new CmdResult{"error: unable to execute a shell command", -1};

class BaseController {
  public:
    BaseController() {}

  protected:
    std::unique_ptr<CmdResult> execute(std::string nordvpn);
};

#endif // BASECONTROLLER_H
