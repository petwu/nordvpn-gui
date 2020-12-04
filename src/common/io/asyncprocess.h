#ifndef ASYNCPROCESS_H
#define ASYNCPROCESS_H

#include <functional>
#include <string>
#include <sys/types.h>

#include "common/io/processresult.h"

/**
 * @brief The AsyncProcess class extends the Process class and allows
 * asynchonous execution of the command.
 */
class AsyncProcess {
  public:
    /**
     * @brief Execute a shell command. The command is executed in a seperate
     * thread, thence the function is non-blocking and will return immediately.
     * Use Process for blocking execution.
     * @param command The shell command to execute.
     */
    static void execute(const std::string &command);

    /**
     * @brief Execute a shell command. The command is executed in a seperate
     * thread, thence the function is non-blocking and will return immediately.
     * Use Process for blocking execution.
     * @param command The shell command to execute.
     * @param pid Pointer to a pid_t variable to write the process ID to. The
     * pid gets written once the child process gets created and is therefore
     * available before the command execution finished. Once the command is
     * finished, pid is set to -1.
     */
    static void execute(const std::string &command, pid_t *pid);

    /**
     * @brief Execute a shell command. The command is executed in a seperate
     * thread, thence the function is non-blocking and will return immediately.
     * Use Process for blocking execution.
     * @param command The shell command to execute.
     * @param pid Pointer to a pid_t variable to write the process ID to. The
     * pid gets written once the child process gets created and is therefore
     * available before the command execution finished. Once the command is
     * finished, pid is set to -1.
     * @param callback The method that gets called once the command has finished
     * and the result is available.
     */
    static void
    execute(const std::string &command, pid_t *pid,
            const std::function<void(const ProcessResult &)> &callback);

    /**
     * @brief Send a termination request in order to kill the child process
     * running the command.
     * @param pid The process ID of the process that should be killed.
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
    static auto kill(pid_t pid, bool force = false) -> bool;
};

#endif // ASYNCPROCESS_H
