#include "asyncprocess.h"

void AsyncProcess::execute(std::function<void(ProcessResult)> resultCallback) {
    std::thread([this, resultCallback] {
        // run command
        auto result = this->_popen3();
        // set killed flag of result since this is not done in the synchronous
        // version where _popen3 is inherited from
        result.killedIntentionally = this->_killed;
        // notify the call as a last action before exiting the thread
        resultCallback(result);
    }).detach();
}

bool AsyncProcess::terminate(bool force) {
    this->_killed = true;
    // check if it is a unique process ID that only terminates one process and
    // not all processes in the process group
    if (this->_pid <= 0)
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
    int rc = kill(this->_pid, force ? SIGKILL : SIGTERM);
    // check if successful
    return rc == EXIT_SUCCESS;
}
