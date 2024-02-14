// SPDX-License-Identifier: MIT
#ifndef SYS_PROCESS_H
#define SYS_PROCESS_H

#include <string>
#include <unistd.h>
#include <vector>

namespace tdm {

class Process
{
  private:
    bool m_started = false;

    //! Executable binary file
    std::string m_binary;

    //! Executable arguments
    std::vector<std::string> m_args;

    //! Child process PID
    pid_t m_pid = 0;

    //! Standard output descriptor
    int m_stdout_fd[2] = {-1};
    FILE *m_stdout = nullptr;

    //! Standard error descriptor
    int m_stderr_fd[2] = {-1};
    FILE *m_stderr = nullptr;

    //! Child process return code
    int m_return_code = -1;

  public: // (Con|De)structors
    // Default constructor
    Process(void) = default;

    // Move constructor
    Process(Process &&o);

    /**
     * Binary string constructor
     *
     * @param bin Initial binary file path
     **/
    Process(std::string bin);

    // Destructor
    ~Process(void);

  public: // Operators
    //! Move assignment
    Process &operator=(Process &&o);

    //! Get the running state of the process
    operator bool(void) const;

  public: // Constant getters
    /**
     * Get internal binary
     *
     * @returns Internal binary
     **/
    const std::string &binary(void) const;

    //! Get child process id
    pid_t pid(void) const;

    //! Standard out descriptor of child process
    FILE *stdout(void) const;

    //! Standard error descriptor of child process
    FILE *stderr(void) const;

    //! Get process return code (post-wait)
    int return_code(void) const;

  public:
    /**
     * Set internal binary
     *
     * @param bin New binary file path
     **/
    Process &binary(const std::string &bin);

    template <typename... Args>
    Process &arg(std::string value, Args &&...args)
    {
        arg(std::move(value));            // arg(value)
        arg(std::forward<Args>(args)...); // arg(value, ...)
        return *this;
    }

    //! Add an argument to executable args
    Process &arg(std::string value);

    /**
     * Start the process
     *
     * @returns Result of fork()
     **/
    pid_t start(void);

    /**
     * Stop the process
     *
     * @param sig Kill signal
     **/
    bool kill(int sig);

    //! Wait for the process to exit
    Process &wait(void);

  private:
    /**
     * Close internal fds and FILE pointers
     **/
    void close(void);

    /**
     * Create a pipe
     *
     * @throws std::runtime_error When receiving an error from pipe()
     **/
    void create_pipe(int *fds);
};
// End of Process decls
} // namespace tdm

#endif /* SYS_PROCESS_H */
