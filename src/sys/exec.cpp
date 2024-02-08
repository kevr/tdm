#include "exec.h"
#include "../util/termio.h"
#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <utility>

namespace tdm {

Exec::Exec(std::string exec) : m_exec(std::move(exec)) {}
Exec::~Exec(void) { close(); }

int Exec::return_code(void) const { return m_return_code; }
const std::string &Exec::error(void) const { return m_error; }

int Exec::operator()(const char *args)
{
    if (int rc = sys->pipe(m_pfd); rc < 0) {
        return set_error(errno);
    }

    if (!(m_stderr = sys->fdopen(m_pfd[0], "r"))) {
        return set_error(errno);
    }

    std::string cmd = fmt::format("{} {} 2>&{}", m_exec, args, m_pfd[1]);
    if (!(m_stdout = sys->popen(cmd.c_str(), "r"))) {
        return set_error(errno);
    }
    ::close(m_pfd[1]);

    int stdout_fd = fileno(m_stdout);
    int stderr_fd = fileno(m_stderr);
    std::vector<int> fds({stdout_fd, stderr_fd});
    for (int fd : fds) {
        if (set_nonblocking(fd) == -1) {
            throw std::runtime_error("fcntl failed");
        }
        m_fds.emplace_back(fd);
    }

    return 0;
}

int Exec::communicate(std::function<void(std::string)> stdout_fn,
                      std::function<void(std::string)> stderr_fn)
{
    // Select loop until the process is done
    fd_set fds;
    FD_ZERO(&fds);

    for (int fd : m_fds) {
        FD_SET(fd, &fds);
    }

    std::map<int, std::function<void(std::string)>> handlers;
    handlers[fileno(m_stdout)] = stdout_fn;
    handlers[fileno(m_stderr)] = stderr_fn;

    struct timeval tv = {.tv_sec = 0, .tv_usec = 1000 * 100};
    int maxfd = *std::max_element(m_fds.begin(), m_fds.end());
    int result = select(maxfd + 1, &fds, nullptr, nullptr, &tv);
    while (m_fds.size() && result != -1) {
        // If we reached timeout
        if (result > 0) {
            for (int fd : m_fds) {
                if (FD_ISSET(fd, &fds)) {
                    read_some(fd, m_streams[fd], m_lines[fd]);

                    std::string line;
                    while (m_lines[fd]) {
                        --m_lines[fd];
                        std::getline(m_streams[fd], line);
                        handlers.at(fd)(std::move(line));
                    }
                }
            }
        }

        tv = {.tv_sec = 0, .tv_usec = 1000 * 100};
        maxfd = *std::max_element(m_fds.begin(), m_fds.end());
        result = select(maxfd + 1, &fds, nullptr, nullptr, &tv);
    }

    return return_code();
}

int Exec::set_nonblocking(int fd)
{
    int flags = sys->fcntl(fd, F_GETFL, 0);
    return sys->fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int Exec::read_some(int fd, std::stringstream &buffer, size_t &lines)
{
    char buf[127 + 1] = {0};

    // Read until we don't encounter EAGAIN
    int bytes = sys->read(fd, buf, 127);
    while (bytes == -1 && errno == EAGAIN) {
        // On bytes == -1 && errno == EAGAIN, keep reading until we get
        // something else
        bytes = sys->read(fd, buf, 127);
    }

    logger.debug("read bytes: {}", bytes);

    // If we got a non-erroneous read
    if (bytes > 0) {
        // then null terminate the buffer
        buf[bytes] = '\0';

        // and append it to the stream
        auto sv = buffer.view();
        auto o = sv.size();
        buffer << buf;
        sv = buffer.view();

        // Scan the newly added data for line endings
        size_t pos = sv.find("\n", o);
        while (pos != std::string::npos) {
            lines++;
            o = pos + 1;
            pos = sv.find("\n", o);
        }

        logger.debug("added buffer to stream for fd {}, lines = {}", fd, lines);
    } else {
        // In this case, bytes was <= 0 and errno != EAGAIN, in which case
        // we close up the child process
        close();
    }

    return bytes;
}

int Exec::set_error(int error)
{
    m_error = strerror(error);
    m_return_code = error;
    return return_code();
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

    ::close(m_pfd[1]), ::close(m_pfd[0]);
    return return_code();
}

} // namespace tdm
