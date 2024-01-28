/**
 * A static handle to various libc and system calls we need to
 * mock for testing purposes.
 **/
#ifndef LIB_SYS_H
#define LIB_SYS_H

#include "../singleton.h"
#include <pwd.h>
#include <sys/types.h>

namespace tdm {

class Sys
{
  public:
    virtual ~Sys(void) = default;

  public:
    virtual struct passwd *getpwuid(uid_t uid) const;
    virtual int fcntl(int, int, int) const;
    virtual int pipe(int fds[2]) const;
    virtual FILE *fdopen(int, const char *) const;
    virtual FILE *popen(const char *, const char *) const;
    virtual int read(int fd, void *buf, size_t len) const;
};

inline Singleton<Sys> sys;
}; // namespace tdm

#endif /* LIB_SYS_H */
