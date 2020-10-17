#ifndef PROCESS_H
#define PROCESS_H

#include <fcntl.h>
#include <paths.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "common/util/strings.h"
#include "processresult.h"

/**
 * @brief The Process class is able to create a child process, execute a
 * command and return it's output and exit information.
 *
 * @attention SECURITY WARNING: The command gets executed by invoking a shell.
 * It's best to not use any user-provided input inside the command to avaoid
 * shell expansion.
 */
class Process {
  public:
    /**
     * @brief Executes a commandin a blocking was. Use AsyncProcess for
     * non-blocking execution.
     * @param command The shell command to exeute.
     * @return The commands output and exit code wrapped inside a ProcessResult
     * object.
     */
    static ProcessResult execute(std::string command);
};

/**
 * @brief Main routine, that creates a fork, executes #_command and
 * retrieves the following information about the process:
 * - process ID (#_pid)
 * - return code
 * - output from stdout
 * - errors from stderr
 * The name is referring to popen() from <stdio.h> that allows opens a
 * process and invokes a shell. This is an extended version that uses 3
 * instead of only 1 pipe: One for each of stdin, stdout, stderr. popen in
 * contrast only uses a pipe for stdin and does therefore allow only
 * unidirectional communication.
 * @param command Variable that contains the command to execute.
 * @param pid Variable to write the process ID to.
 * @return The child process exit information (return code, output, error,
 * etc.) as ProcessResult object.
 */
ProcessResult popen3(std::string command, pid_t *pid);

#endif // PROCESS_H
