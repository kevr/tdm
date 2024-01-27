#include "exec.h"
#include "../util/termio.h"

namespace tdm {

ssize_t getline(FILE *fd, std::string &line)
{
    if (fd == nullptr)
        return -1;

    char *c_line = NULL;
    size_t len = 0;
    ssize_t bytes;
    if ((bytes = ::getline(&c_line, &len, fd)); bytes > 0) {
        line.resize(bytes);
        memcpy(line.data(), c_line, bytes);
    }
    free(c_line);

    return bytes;
}

Exec::Exec(std::string exec) : m_exec(std::move(exec)) {}
Exec::~Exec(void) { close(); }

int Exec::return_code(void) const { return m_return_code; }
const std::string &Exec::error(void) const { return m_error; }

int Exec::operator()(const char *args)
{
    if (int rc = sys->pipe(pfd); rc < 0) {
        return set_error(errno);
    }

    if (!(m_stderr = sys->fdopen(pfd[0], "r"))) {
        return set_error(errno);
    }

    std::string cmd = std::format("{} {} 2>&{}", m_exec, args, pfd[1]);
    if (!(m_stdout = sys->popen(cmd.c_str(), "r"))) {
        return set_error(errno);
    }
    ::close(pfd[1]);

    int stdout_fd = set_nonblocking(fileno(m_stdout));
    select_fds.emplace_back(stdout_fd);

    int stderr_fd = set_nonblocking(fileno(m_stderr));
    select_fds.emplace_back(stderr_fd);

    return 0;
}

int Exec::communicate(std::function<void(std::string)> stdout_fn,
                      std::function<void(std::string)> stderr_fn)
{
    auto fn = [this](FILE *f, auto fn_) {
        std::string line;
        if (getline(f, line) > 0) {
            fn_(std::move(line));
        } else {
            close();
        }
    };
    auto stdout_fn_ = [fn, stdout_fn](FILE *f) { fn(f, stdout_fn); };
    auto stderr_fn_ = [fn, stderr_fn](FILE *f) { fn(f, stderr_fn); };
    while (select(stdout_fn_, stderr_fn_) != -1)
        ;
    return return_code();
}

int Exec::set_error(int error)
{
    m_error = strerror(error);
    m_return_code = errno;
    return return_code();
}

int Exec::set_nonblocking(int fd)
{
    int flags = sys->fcntl(fd, F_GETFL, 0);
    if (int rc = sys->fcntl(fd, F_SETFL, flags | O_NONBLOCK); rc == -1) {
        m_error = strerror(errno);
        throw std::runtime_error(m_error);
    }
    return fd;
}

int Exec::select(std::function<void(FILE *)> stdout_fn,
                 std::function<void(FILE *)> stderr_fn)
{
    if (!m_stdout)
        return -1;

    int stdout_fd = fileno(m_stdout);
    int stderr_fd = fileno(m_stderr);

    fd_set read_fds;
    FD_ZERO(&read_fds);
    int max_fd = -1;
    for (auto select_fd : select_fds) {
        FD_SET(select_fd, &read_fds);
        if (select_fd > max_fd)
            max_fd = select_fd;
    }

    struct timeval tv = {.tv_sec = 0, .tv_usec = 1000 * 1000};
    int result = ::select(max_fd + 1, &read_fds, nullptr, nullptr, &tv);
    if (result > 0) {
        if (FD_ISSET(stdout_fd, &read_fds)) {
            stdout_fn(m_stdout);
        } else if (FD_ISSET(stderr_fd, &read_fds)) {
            stderr_fn(m_stderr);
        }
    }

    return result;
}

int Exec::close(void)
{
    if (m_stderr) {
        fclose(m_stderr);
        m_stderr = nullptr;
    }

    if (m_stdout) {
        m_return_code = WEXITSTATUS(pclose(m_stdout));
        m_stdout = nullptr;
    }

    ::close(pfd[1]), ::close(pfd[0]);
    return return_code();
}

} // namespace tdm
