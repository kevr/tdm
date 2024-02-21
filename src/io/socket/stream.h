// SPDX-License-Identifier: MIT
#ifndef IO_SOCKET_STREAM_H
#define IO_SOCKET_STREAM_H

#include "../../util/logger.h"
#include "basic_stream.h"

namespace tdm::io::socket {

class Stream : public BasicStream
{
  public:
    using BasicStream::BasicStream;

  public:
    Stream(int fd);

  public:
    //! Derivates should override getline
    int getline(std::string &line) override;

    //! Soup the entire fd into line
    std::string soup(void);
};

// End of Stream
} // namespace tdm::io::socket

#endif /* IO_SOCKET_STREAM_H */
