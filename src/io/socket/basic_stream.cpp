// SPDX-License-Identifier: MIT
#include "basic_stream.h"

namespace tdm::io::socket {

BasicStream::BasicStream(int fd) : m_fd(fd)
{
}

BasicStream::BasicStream(BasicStream &&o)
{
    operator=(std::move(o));
}

BasicStream &BasicStream::operator=(BasicStream &&o)
{
    // Move file descriptor
    m_fd = o.m_fd;
    o.m_fd = -1;

    // Move buffer
    m_buffer = std::move(o.m_buffer);

    return *this;
}

int BasicStream::fd(void) const
{
    return m_fd;
}

int BasicStream::handle_read(char *buffer, int bytes)
{
    if (bytes > 0) {
        buffer[bytes] = '\0';
        m_buffer << buffer;
    }
    return bytes;
}

} // namespace tdm::io::socket
