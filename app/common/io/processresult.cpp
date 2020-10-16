#include "processresult.h"

ProcessResult::ProcessResult(std::string cmd, std::string out, std::string err,
                             uint32_t rc)
    : command(cmd), output(out), error(err), exitCode(rc) {}

bool ProcessResult::operator==(const ProcessResult &other) const {
    return this->exitCode == other.exitCode && this->output == other.output &&
           this->error == other.error;
}

bool ProcessResult::operator!=(const ProcessResult &other) const {
    return !(*this == other);
}

bool ProcessResult::success() { //
    return this->exitCode == EXIT_SUCCESS;
}

#include "common/util/strings.h"

std::string ProcessResult::toString() {
    auto err = util::string::replaceAll(this->error, "\n", "\n             ");
    auto out = util::string::replaceAll(this->output, "\n", "\n             ");
    return std::string("ProcessResult {") +                                 //
           "\n  command  = " + this->command +                              //
           "\n  exitCode = " + std::to_string(this->exitCode) +             //
           "\n  killed   = " + (this->killedIntentionally ? "yes" : "no") + //
           "\n  stderr   = " + err +                                        //
           "\n  stdout   = " + out +                                        //
           "\n}";
}
