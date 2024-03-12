// SPDX-License-Identifier: MIT
#include "sys.h"
#include <cstdio>
#include <fcntl.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

namespace tdm::lib {

struct passwd *Sys::getpwnam(const char *name)
{
    return ::getpwnam(name);
}

struct passwd *Sys::getpwuid(uid_t uid)
{
    return ::getpwuid(uid);
}

// dup2/execve are never used in the main process, so coverage is excluded
// LCOV_EXCL_START
int Sys::dup2(int fd, int fd2)
{
    return ::dup2(fd, fd2);
}

int Sys::execve(const char *path, char *const *argv, char *const *env)
{
    return ::execve(path, argv, env);
}
// LCOV_EXCL_STOP

bool Sys::exists(const std::filesystem::path &path)
{
    return std::filesystem::exists(path);
}

int Sys::fcntl(int fd, int cmd, int arg)
{
    return ::fcntl(fd, cmd, arg);
}

pid_t Sys::fork(void)
{
    return ::fork();
}

int Sys::setegid(gid_t gid)
{
    return ::setegid(gid);
}

int Sys::seteuid(uid_t uid)
{
    return ::seteuid(uid);
}

int Sys::kill(pid_t pid, int sig)
{
    return ::kill(pid, sig);
}

int Sys::mkdir(const char *path, mode_t mode)
{
    return ::mkdir(path, mode);
}

int Sys::pipe(int *pipedes)
{
    return ::pipe(pipedes);
}

int Sys::read(int fd, void *buf, size_t count)
{
    return ::read(fd, buf, count);
}

int Sys::waitpid(pid_t pid, int *status, int flags)
{
    return ::waitpid(pid, status, flags);
}

} // namespace tdm::lib
