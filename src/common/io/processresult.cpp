#include "processresult.hpp"

#include <type_traits>
#include <utility>

ProcessResult::ProcessResult(std::string cmd, std::string out, std::string err,
                             uint32_t rc)
    : command(std::move(std::move(cmd))), output(std::move(std::move(out))),
      error(std::move(std::move(err))), exitCode(rc) {}

auto ProcessResult::operator==(const ProcessResult &other) const -> bool {
    return this->exitCode == other.exitCode && this->output == other.output &&
           this->error == other.error;
}

auto ProcessResult::operator!=(const ProcessResult &other) const -> bool {
    return !(*this == other);
}

auto ProcessResult::success() const -> bool { //
    return this->exitCode == EXIT_SUCCESS;
}

#include "common/util/strings.hpp"

auto ProcessResult::toString() const -> std::string {
    auto err = util::string::replaceAll(this->error, "\n", "\n             ");
    auto out = util::string::replaceAll(this->output, "\n", "\n             ");
    return std::string("ProcessResult {") +                     //
           "\n  command  = " + this->command +                  //
           "\n  exitCode = " + std::to_string(this->exitCode) + //
           "\n  stderr   = " + err +                            //
           "\n  stdout   = " + out +                            //
           "\n}";
}
