// SPDX-License-Identifier: MIT
#include "process.h"
#include "../lib/sys.h"
#include "../util/filesystem.h"
#include "../util/logger.h"
#include "../util/str.h"
#include <fmt/format.h>
#include <signal.h>
#include <sys/wait.h>

namespace tdm {

Process::Process(Process &&o)
{
    operator=(std::move(o));
}

Process::Process(std::string bin)
{
    binary(bin);
}

Process::~Process(void)
{
    kill(SIGTERM);
}

Process &Process::operator=(Process &&o)
{
    m_started = o.m_started;
    o.m_started = false;

    m_binary = std::move(o.m_binary);
    m_args = std::move(o.m_args);

    m_pid = o.m_pid;
    o.m_pid = 0;

    m_stdout_fd[0] = o.m_stdout_fd[0];
    o.m_stdout_fd[0] = -1;

    m_stdout_fd[1] = o.m_stdout_fd[1];
    o.m_stdout_fd[1] = -1;

    m_stdout = o.m_stdout;
    o.m_stdout = nullptr;

    m_stderr_fd[0] = o.m_stderr_fd[0];
    o.m_stderr_fd[0] = -1;

    m_stderr_fd[1] = o.m_stderr_fd[1];
    o.m_stderr_fd[1] = -1;

    m_stderr = o.m_stderr;
    o.m_stderr = nullptr;

    m_return_code = o.m_return_code;
    return *this;
}

Process::operator bool(void) const
{
    return m_started;
}

const std::string &Process::binary(void) const
{
    return m_binary;
}

pid_t Process::pid(void) const
{
    return m_pid;
}

FILE *Process::stdout(void) const
{
    return m_stdout;
}

FILE *Process::stderr(void) const
{
    return m_stderr;
}

int Process::return_code(void) const
{
    return m_return_code;
}

Process &Process::binary(const std::string &bin)
{
    m_args.clear();
    m_binary = bin;
    return *this;
}

Process &Process::arg(std::string value)
{
    m_args.emplace_back(std::move(value));
    return *this;
}

pid_t Process::start(void)
{
    if (*this) {
        std::string message =
            fmt::format("process has already started with pid {}", m_pid);
        throw std::logic_error(message);
    } else if (!m_binary.size()) {
        throw std::invalid_argument("no binary provided");
    }

    // Binary found in $PATH
    auto binary_path = search_path(m_binary);

    // Create child process stdout/stderr pipes
    create_pipe(m_stdout_fd);
    m_stdout = sys->fdopen(m_stdout_fd[0], "r");
    create_pipe(m_stderr_fd);
    m_stderr = sys->fdopen(m_stderr_fd[0], "r");

    m_pid = sys->fork();
    if (m_pid == -1) {
        // Error
        close();
        return pid();
    } else if (m_pid > 0) {
        // In parent
        m_started = true;
        return pid();
    }

    // LCOV_EXCL_START
    // In child
    sys->dup2(m_stdout_fd[1], STDOUT_FILENO);
    sys->dup2(m_stderr_fd[1], STDERR_FILENO);

    std::vector<char *> v;
    v.emplace_back(m_binary.data());
    for (auto &arg : m_args)
        v.emplace_back(arg.data());
    v.emplace_back(nullptr);

    sys->execve(binary_path.c_str(), v.data(), nullptr);

    logger.error("unable to execve {}, error = {}", m_binary, strerror(errno));
    return pid();
    // LCOV_EXCL_STOP
}

bool Process::kill(int sig)
{
    int rc = -1;
    if (m_started) {
        close();
        rc = sys->kill(m_pid, sig);
        m_pid = 0;
        m_started = false;
    }
    return rc == 0;
}

Process &Process::wait(void)
{
    int status;
    do {
        if (sys->waitpid(m_pid, &status, 0) == -1) {
            std::string msg =
                fmt::format("waitpid() failed, error = {}", strerror(errno));
            throw std::runtime_error(msg);
        }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));

    // TODO: Figure out how to set these properly...
    if (WIFEXITED(status)) {
        m_return_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        m_return_code = WTERMSIG(status);
    }

    return *this;
}

void Process::close(void)
{
    // Close child stderr pipe handle and fds
    if (m_stderr) {
        ::fclose(m_stderr);
        m_stderr = nullptr;
    }
    ::close(m_stderr_fd[0]);
    ::close(m_stderr_fd[1]);

    // Close child stdout pipe handle and fds
    if (m_stdout) {
        ::fclose(m_stdout);
        m_stdout = nullptr;
    }
    ::close(m_stdout_fd[0]);
    ::close(m_stdout_fd[1]);
}

void Process::create_pipe(int *fds)
{
    if (sys->pipe(fds) == -1) {
        throw std::runtime_error("failed to create process pipe");
    }
}

// End of Process defs
} // namespace tdm
