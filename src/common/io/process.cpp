#include "process.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <paths.h>
#include <sys/wait.h>
#include <utility>

#include "common/io/processresult.h"
#include "common/util/strings.h"

// define reading and writing ends of a pipe
constexpr unsigned int READ = 0;
constexpr unsigned int WRITE = 1;

auto Process::execute(const std::string &command) -> ProcessResult {
    // use 3 pipes and fork+exec to run the _command
    return popen3(command, nullptr);
}

auto popen3(const std::string &command, pid_t *pid) -> ProcessResult {
    // return values
    int rc = EXIT_SUCCESS;
    std::string out;
    std::string err;

    // init pipes
    // every pipe is a array of two file descriptors/handles represented as
    // integers:
    //   pipe[0] --> reading end / pipe output
    //   pipe[1] --> writing end / pipe input
    std::array<int, 2> pipeIn{};
    std::array<int, 2> pipeOut{};
    std::array<int, 2> pipeErr{};
    if (pipe(pipeIn.data()) != 0 || pipe(pipeOut.data()) != 0 ||
        pipe(pipeErr.data()) != 0) {
        return ProcessResult{command, "", "error: unable to create a pipe",
                             EXIT_FAILURE};
    }

    // fork child process and remember its process ID
    pid_t _pid = fork();
    if (pid != nullptr) {
        *pid = _pid;
    }
    if (_pid < 0) {

        // --> fork failed
        return ProcessResult{command, "", "error: unable to fork a subprocess",
                             EXIT_FAILURE};
    }
    if (_pid == 0) {

        // --> code that is run inside the new/child process

        // close writing end of input pipe since it is not reaquired by the
        // child process
        close(pipeIn[WRITE]);
        // duplicate input pipe to stdin
        dup2(pipeIn[READ], STDIN_FILENO);

        // close reading end of output pipe since it is not reaquired by the
        // child process
        close(pipeOut[READ]);
        // duplicate output pipe to stdout
        dup2(pipeOut[WRITE], STDOUT_FILENO);

        // close reading end of error pipe since it is not reaquired by the
        // child process
        close(pipeErr[READ]);
        // duplicate error pipe to stderr
        dup2(pipeErr[WRITE], STDERR_FILENO);

        // execute as shell command
        // execl cannot be used without varargs, hence the disabled clang-tidy
        // checks NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        execl(_PATH_BSHELL, _PATH_BSHELL, "-c", command.c_str(), NULL);

        // the exec() family of functions replaces the current process image
        // with a new process image, which means the following code should not
        // be executed
        perror(("error executing '" + command + "' in child process").c_str());
        exit(EXIT_FAILURE);

    } else {

        // --> code that is run inside the current/parent process

        // close not needed pipes
        close(pipeIn[READ]);
        close(pipeOut[WRITE]);
        close(pipeErr[WRITE]);

        // wait for child process to finish and retrieve its return code
        wait(&rc);

        // read out and err strings from the corresponding pipes.
        // the data is written into the pipes by the child process
        std::array<char, 256> buf{}; // NOLINT(readability-magic-numbers,
                                     // cppcoreguidelines-avoid-magic-numbers):
                                     // buffer size is not a critical number
        int r = 0;
        while ((r = read(pipeOut[READ], buf.data(), buf.size())) > 0) {
            out.append(buf.data(), 0, r);
        }
        while ((r = read(pipeErr[READ], buf.data(), buf.size())) > 0) {
            err.append(buf.data(), 0, r);
        }

        // final clean-up
        close(pipeIn[WRITE]);
        close(pipeOut[READ]);
        close(pipeErr[READ]);
    }

    return std::move(ProcessResult{
        command,
        util::string::trim(out),
        util::string::trim(err),
        static_cast<uint32_t>(WEXITSTATUS(rc)),
    });
}
