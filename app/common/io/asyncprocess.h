#ifndef ASYNCPROCESS_H
#define ASYNCPROCESS_H

#include <functional>

#include "process.h"

/**
 * @brief The AsyncProcess class extends the Process class and allows
 * asynchonous execution of the command.
 */
class AsyncProcess : public Process {
  public:
    using Process::Process;
    /**
     * @brief Executes the command passed to the constructor. The command is
     * execution in a seperate thread, hence the function is non-blocking and
     * will return immediately. Use Process for blocking execution.
     * @param resultCallback The callback method that gets called once the
     * command has finished and the result is available.
     */
    void execute(std::function<void(const ProcessResult)> resultCallback);

    /**
     * @brief Send a termination request in order to kill the child process
     * running the command.
     * @param force If true, the kill request sends the signal SIGKILL,
     * otherwise SIGTERM. Difference:
     * - SIGTERM is more polite. The process has the possibily to handle the
     * signal and usually does some clean-up -- but it does not have to, is
     * might ignore the signal an continue execution as well.
     * - SIGKILL is more brutal. The process is not aware of the signal since it
     * goes straight to the kernel which stops the process, hence no clean-up
     * can be done but the termination is guaranteed.
     * Reccomendation: Use false (SIGTERM) if you know for sure that the called
     * application respects the signal, true (SIGKILL) otherwise.
     * @return True, if the child process was killed sucessfully. False,
     * otherwise.
     */
    bool terminate(bool force = false);

  private:
    // hide the public execute method of the base class
    using Process::execute;

    /**
     * @brief Flag, that gets set to true when #terminal gets called. It
     * therefore is only true, if the kill comes from within this application. A
     * kill from outside (e.g. if the user kills the process in the task manager
     * / system monitor) the kill will _not_ be detectable through this flag.
     */
    bool _killed = false;
};

#endif // ASYNCPROCESS_H
