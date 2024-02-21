// SPDX-License-Identifier: MIT
#ifndef IO_SOCKET_NONBLOCKING_STREAM_H
#define IO_SOCKET_NONBLOCKING_STREAM_H

#include "basic_stream.h"

namespace tdm::io::socket {

class NonBlockingStream : public BasicStream
{
  private:
    // File descriptors used during select()
    fd_set m_fds;

    // Timeout in useconds
    long m_timeout;

  public:
    // Inherit BasicStream's constructors
    using BasicStream::BasicStream;

  public:
    NonBlockingStream(int fd);

  public:
    //! Set select() timeout
    NonBlockingStream &timeout(long usec);

  public:
    /**
     * Try to get a line and cancel on reaching usec timeout.
     *
     * @param line External std::string reference
     * @param usec Timeout in useconds (default: 3 seconds)
     * @returns line.size() if successful, otherwise -1
     * @throws std::out_of_range If timeout is reached
     **/
    int getline(std::string &line) override;

  private:
    //! Read some data from m_fd into m_buffer
    int readsome(void);

    //! @throws std::runtime_error if fcntl fails
    int fcntl(int fd, int cmd, int arg);
};

} // namespace tdm::io::socket

#endif /* IO_SOCKET_NONBLOCKING_STREAM_H */
