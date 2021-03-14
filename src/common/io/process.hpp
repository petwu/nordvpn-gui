#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <unistd.h>

#include "common/io/processresult.hpp"

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
    static auto execute(const std::string &command) -> ProcessResult;
};

/**
 * @brief Function to execute a shell command.
 *
 * @details Routine, that creates a fork, executes `command` and retrieves the
 * following information about the process:
 *
 * - process ID (`pid`)
 * - return code
 * - output from stdout
 * - errors from stderr
 *
 * The name is referring to popen() from \<cstdio> that allows opens a
 * process and invokes a shell. This is an extended version that uses 3
 * instead of only 1 pipe: One for each of stdin, stdout, stderr. popen in
 * contrast only uses a pipe for stdin and does therefore allow only
 * unidirectional communication.
 *
 * @attention SECURITY WARNING: The command gets executed by invoking a shell.
 * It's best to not use any user-provided input inside the command to avaoid
 * shell expansion.
 *
 * @startuml
 * start
 * :create 3 pipe()s for stdin, stdout and stderr;
 * if (failed ?) then (yes)
 *   #pink:return error;
 *   end
 * endif
 * :fork() a new child process;
 * :set <i>pid</i> to the childs process ID;
 * if (<i>pid</i> < 0 ?) then (yes)
 *   #pink:return error;
 *   end
 * endif
 *   floating note right
 *     After the fork(), the following code, despite
 *     being a simple if-else branching, does not
 *     continue straight-forward as usual.
 *     Some of it runs in a seperate child process
 *     (left) and some continues in the
 *      current/parent process (right).
 *   end note
 * fork
 *   -[#transparent]->
 *   start
 *   :close the not used pipe ends;
 *   floating note right
 *     ↓ child process
 *   end note
 *   :set (duplicate) the used pipe ends
 *   to stdin, stdout and stderr of the child;
 *   :exec() <i>command</i> on shell;
 *   if (exec() works ?) then (yes)
 *     stop
 *   endif
 *   -[dotted]-> \n;
 *   #pink:exit with failure;
 *   note right
 *     this action should never be reached,
 *     since the exec() family of functions
 *     replaces the current process image
 *   end note
 *   end
 * fork again
 *   :close the not used pipe ends;
 *   floating note left
 *     parent process ↓
 *   end note
 *   :wait for the child process to terminate;
 *   -[dashed]-> \n\n\n\n\n\n;
 *   :get return code;
 *   :read from stdout and stderr;
 *   :close remaining open pipe ends;
 * endfork
 * #palegreen:return result;
 * stop
 * @enduml
 *
 * @param command Variable that contains the command to execute.
 * @param[out] pid Variable to write the process ID to.
 * @return The child process exit information (return code, output, error,
 * etc.) as ProcessResult object.
 */
auto popen3(const std::string &command, pid_t *pid) -> ProcessResult;

#endif // PROCESS_HPP
