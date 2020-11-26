#ifndef PROCESSRESULT_H
#define PROCESSRESULT_H

#include <string>

/**
 * @brief The ProcessResult class is a data class that contains the result of an
 * external process. See Process and AsyncProcess for classes that are able to
 * execute a process and return the approriate ProcessResult.
 */
class ProcessResult {
  public:
    /**
     * @brief Only constructor of ProcessResult.
     * @param cmd The executed command.
     * @param out The commands default output (stdout). May be empty.
     * @param err The commands error output (stderr). May be empty.
     * @param rc The commands return/exit code.
     */
    ProcessResult(std::string cmd, std::string out, std::string err,
                  uint32_t rc);

    /** @brief The command that was executed. */
    std::string command = "";
    /** @brief The output returned to stdout by the command. */
    std::string output = "";
    /** @brief The output returned to stderr by the command. */
    std::string error = "";
    /** @brief The exit code returned by the command. */
    uint32_t exitCode = EXIT_SUCCESS;

    bool operator==(const ProcessResult &other) const;
    bool operator!=(const ProcessResult &other) const;

    /**
     * @brief success
     * @return True if #exitCode indicates a successful termination, false
     * otherwise.
     */
    bool success();

    /**
     * @brief toString
     * @return The object represented as a string containing all (public) member
     * variables.
     */
    std::string toString();

    // delete copy constructor -> only move semantics
    ProcessResult(const ProcessResult &c) = default;
    ProcessResult &operator=(const ProcessResult &) = default;
    ProcessResult(ProcessResult &&m) = default;
    ProcessResult &operator=(ProcessResult &&) = default;

  private:
    ProcessResult() = default;
};

#endif // PROCESSRESULT_H
