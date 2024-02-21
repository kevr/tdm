// SPDX-License-Identifier: MIT
#ifndef IO_SOCKET_BASIC_STREAM_H
#define IO_SOCKET_BASIC_STREAM_H

#include <sstream>
#include <string>

namespace tdm::io::socket {

class BasicStream
{
  protected:
    static const int BUFFER_LEN = 256;

  protected:
    int m_fd = -1;
    std::stringstream m_buffer;

  public:
    virtual ~BasicStream(void) = default;
    virtual int getline(std::string &line) = 0;

  public:
    BasicStream(int fd);
    BasicStream(BasicStream &&o);

  public:
    BasicStream &operator=(BasicStream &&o);
    int fd(void) const;

  protected:
    int handle_read(char *buffer, int bytes);
};

} // namespace tdm::io::socket

#endif /* IO_SOCKET_BASIC_STREAM_H */
