#include "asyncprocess.h"

#include <csignal>
#include <cstdlib>
#include <thread>

#include "common/io/process.h"

void AsyncProcess::execute(const std::string &command) { //
    execute(command, nullptr);
}

void AsyncProcess::execute(const std::string &command, pid_t *pid) {
    execute(command, pid, [](const ProcessResult & /*unused*/) {});
}

void AsyncProcess::execute(
    const std::string &command, pid_t *pid,
    const std::function<void(const ProcessResult &)> &resultCallback) {
    std::thread([command, pid, resultCallback] {
        // run command
        auto result = popen3(command, pid);
        // reset the pid since the child process has terminated when popen3
        // returns and hence the pid is no longer valid
        if (pid != nullptr) {
            *pid = -1;
        }
        // notify the call as a last action before exiting the thread
        resultCallback(result);
    }).detach();
}

auto AsyncProcess::kill(pid_t pid, bool force) -> bool {
    // check if it is a unique process ID that only terminates one process and
    // not all processes in the process group
    if (pid <= 0) {
        return false;
    }
    /*
     * SIGTERM -- more polite: the process has the possibily to handle the
     * signal and usually does some clean-up, but it does not have to and may
     * continue as well
     *
     * SIGKILL -- brutal: the process is not aware of the signal since it goes
     * straight to the kernel which stops the process, hence no clean-up can be
     * done but the termination is guaranteed
     */
    int rc = ::kill(pid, force ? SIGKILL : SIGTERM);
    return rc == EXIT_SUCCESS;
}
