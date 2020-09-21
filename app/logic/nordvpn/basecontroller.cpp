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
    // trim whitespace
    result.erase(result.begin(), std::find_if(result.begin(), result.end(),
                                              [](unsigned char c) {
                                                  return !std::isspace(c) &&
                                                         c != '-';
                                              }));
    result.erase(std::find_if(result.rbegin(), result.rend(),
                              [](unsigned char c) { return !std::isspace(c); })
                     .base(),
                 result.end());
    int rc = WEXITSTATUS(pclose(pipe));
    return std::unique_ptr<CmdResult>(
        new CmdResult{.output = result, .exitCode = rc});
}
