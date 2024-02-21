// SPDX-License-Identifier: MIT
#include "stream.h"
#include <unistd.h>

namespace tdm::io::socket {

Stream::Stream(int fd) : BasicStream(fd)
{
}

int Stream::getline(std::string &line)
{
    char buf[BUFFER_LEN];

    int rc = read(m_fd, buf, BUFFER_LEN - 1);
    while (rc > 0) {
        handle_read(buf, rc);
        if (strchr(buf, '\n'))
            break;
        rc = read(m_fd, buf, BUFFER_LEN - 1);
    }

    if (std::getline(m_buffer, line)) {
        return line.size();
    }

    return rc;
}

std::string Stream::soup(void)
{
    std::string line, doc;
    while (getline(line) > 0) {
        doc.append(std::move(line));
        doc.push_back('\n');
    }
    return doc;
}

} // namespace tdm::io::socket
