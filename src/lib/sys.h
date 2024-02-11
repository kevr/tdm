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
    virtual int dup2(int, int) const;
    virtual int execve(const char *, char *const *argv, char *const *env) const;
    virtual pid_t fork(void) const;
    virtual int kill(pid_t pid, int sig) const;
    virtual int pipe(int fds[2]) const;
    virtual FILE *fdopen(int, const char *) const;
    virtual int waitpid(pid_t pid, int *status, int flags) const;
};

inline Singleton<Sys> sys;
}; // namespace tdm

#endif /* LIB_SYS_H */
