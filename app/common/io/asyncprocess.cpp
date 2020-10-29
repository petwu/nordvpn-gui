#include "asyncprocess.h"

void AsyncProcess::execute(std::string command) { //
    execute(command, nullptr);
}

void AsyncProcess::execute(std::string command, pid_t *pid) {
    execute(command, pid, [](ProcessResult) {});
}

void AsyncProcess::execute(std::string command, pid_t *pid,
                           std::function<void(ProcessResult)> resultCallback) {
    std::thread([command, pid, resultCallback] {
        // run command
        auto result = popen3(command, pid);
        // reset the pid since the child process has terminated when popen3
        // returns and hence the pid is no longer valid
        if (pid != nullptr)
            *pid = -1;
        // notify the call as a last action before exiting the thread
        resultCallback(result);
    }).detach();
}

bool AsyncProcess::kill(pid_t pid, bool force) {
    // check if it is a unique process ID that only terminates one process and
    // not all processes in the process group
    if (pid <= 0)
        return false;
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
