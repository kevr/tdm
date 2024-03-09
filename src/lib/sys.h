// SPDX-License-Identifier: MIT
/**
 * A static handle to various libc and system calls we need to
 * mock for testing purposes.
 **/
#ifndef LIB_SYS_H
#define LIB_SYS_H

#include "../singleton.h"
#include <fcntl.h>
#include <pwd.h>
#include <sys/types.h>

namespace tdm {

class Sys
{
  public:
    virtual ~Sys(void) = default;
    virtual struct passwd *getpwnam(const char *name);
    virtual struct passwd *getpwuid(uid_t uid);
    virtual int dup2(int, int);
    virtual int execve(const char *, char *const *argv, char *const *env);
    virtual int fcntl(int, int, int);
    virtual pid_t fork(void);
    virtual int setegid(gid_t gid);
    virtual int seteuid(uid_t uid);
    virtual int kill(pid_t pid, int sig);
    virtual int pipe(int fds[2]);
    virtual int read(int fd, void *buf, size_t count);
    virtual int waitpid(pid_t pid, int *status, int flags);
};

inline Singleton<Sys> sys;
}; // namespace tdm

#endif /* LIB_SYS_H */
