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
     * @brief Process
     * @param command The command to exeute when calling #execute().
     */
    Process(std::string command);
    /**
     * @brief Process
     * @param command The command to exeute when calling #execute().
     * @param argv Optional arguments that are appended to the executed command.
     */
    Process(std::string command, std::vector<std::string> argv);

    /**
     * @brief Executes the command passed to the constructor. The execution is
     * blocking. Use AsyncProcess for non-blocking execution.
     * @return The commands output and exit code wrapped inside a ProcessResult
     * object.
     */
    ProcessResult execute();

  protected:
    /** @brief String containing the full command including possible args. */
    std::string _command;
    /** @brief ID of the process starten with #_popen3(). */
    pid_t _pid;

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
     */
    ProcessResult _popen3();

  private:
    ProcessResult _error(std::string msg);
};

#endif // PROCESS_H
