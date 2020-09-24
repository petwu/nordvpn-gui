#include "basecontroller.h"

CmdResult::CmdResult(std::string out, uint32_t rc)
    : output(out), exitCode(rc) {}

bool CmdResult::operator==(const CmdResult &other) const {
    return output == other.output && exitCode == other.exitCode;
}

bool CmdResult::operator!=(const CmdResult &other) const {
    return !(*this == other);
}

CmdResult BaseController::execute(std::string cmd) {
    std::array<char, 256> buf;
    std::string result;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        pclose(pipe);
        return ERROR_POPEN_FAILED;
    }
    while (fgets(buf.data(), buf.size(), pipe) != nullptr) {
        result += buf.data();
    }
    result = util::string::trim(result);
    uint32_t rc = WEXITSTATUS(pclose(pipe));
    return std::move(CmdResult{
        .output = result,
        .exitCode = rc,
    });
}

void BaseController::executeNonBlocking(std::string cmd) {
    std::thread t([cmd] {
        std::array<char, 256> buf;
        FILE *pipe = popen(cmd.c_str(), "r");
        if (!pipe) {
            pclose(pipe);
            return;
        }
        while (fgets(buf.data(), buf.size(), pipe) != nullptr) {
        }
        pclose(pipe);
    });
    t.detach();
}
