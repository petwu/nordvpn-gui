#include "basecontroller.h"

std::unique_ptr<CmdResult> BaseController::execute(std::string cmd) {
    std::array<char, 256> buf;
    std::string result;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        pclose(pipe);
        return std::unique_ptr<CmdResult>(ERROR_POPEN_FAILED);
    }
    while (fgets(buf.data(), buf.size(), pipe) != nullptr) {
        result += buf.data();
    }
    result = util::string::trim(result);
    int rc = WEXITSTATUS(pclose(pipe));
    return std::unique_ptr<CmdResult>(
        new CmdResult{.output = result, .exitCode = rc});
}
