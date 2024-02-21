// SPDX-License-Identifier: MIT
#include "nonblocking_stream.h"
#include "../../lib/sys.h"
#include <fmt/format.h>
#include <sys/select.h>
#include <unistd.h>

namespace tdm::io::socket {

NonBlockingStream::NonBlockingStream(int fd) : BasicStream(fd)
{
    int flags = this->fcntl(m_fd, F_GETFL, 0);

    // If m_fd has not had O_NONBLOCK set on it...
    if (!(flags & O_NONBLOCK)) {
        // Then add O_NONBLOCK to socket flags
        this->fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
    }

    // Default timeout to 1 second
    timeout(1000 * 1000);
}

int NonBlockingStream::getline(std::string &line)
{
    FD_ZERO(&m_fds);
    FD_SET(m_fd, &m_fds);

    int result;
    do {
        timeval tv = {.tv_sec = 0, .tv_usec = m_timeout};
        result = select(m_fd + 1, &m_fds, nullptr, nullptr, &tv);

        if (result > 0) {
            if (FD_ISSET(m_fd, &m_fds)) {
                if (this->readsome() <= 0)
                    break;

                if (std::getline(m_buffer, line))
                    return line.size();
            }
        }

        FD_SET(m_fd, &m_fds);
    } while (result > 0);

    if (result == 0) {
        std::string message = fmt::format("select({}) timed out", m_fd);
        throw std::out_of_range(message);
    }

    return -1;
}

NonBlockingStream &NonBlockingStream::timeout(long usec)
{
    m_timeout = usec;
    return *this;
}

int NonBlockingStream::readsome(void)
{
    char buf[BUFFER_LEN];

    int bytes = sys->read(m_fd, buf, BUFFER_LEN - 1);
    while (bytes == -1 && errno == EAGAIN) {
        bytes = sys->read(m_fd, buf, BUFFER_LEN - 1);
    }

    return handle_read(buf, bytes);
}

int NonBlockingStream::fcntl(int fd, int cmd, int arg)
{
    int result = sys->fcntl(fd, cmd, arg);
    if (result == -1) {
        std::string message =
            fmt::format("fcntl failed with error = {}", strerror(errno));
        throw std::runtime_error(message);
    }
    return result;
}

} // namespace tdm::io::socket
