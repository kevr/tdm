#include "sys.h"
#include <cstdio>
#include <fcntl.h>
#include <pwd.h>
#include <sys/wait.h>
#include <unistd.h>

namespace tdm {

struct passwd *Sys::getpwuid(uid_t uid) const { return ::getpwuid(uid); }

// dup2/execve are never used in the main process, so coverage is excluded
// LCOV_EXCL_START
int Sys::dup2(int fd, int fd2) const
{
    return ::dup2(fd, fd2);
}

int Sys::execve(const char *path, char *const *argv, char *const *env) const
{
    return ::execve(path, argv, env);
}
// LCOV_EXCL_STOP

pid_t Sys::fork(void) const
{
    return ::fork();
}

int Sys::kill(pid_t pid, int sig) const
{
    return ::kill(pid, sig);
}

int Sys::pipe(int *pipedes) const { return ::pipe(pipedes); }

FILE *Sys::fdopen(int fd, const char *modes) const
{
    return ::fdopen(fd, modes);
}

int Sys::waitpid(pid_t pid, int *status, int flags) const
{
    return ::waitpid(pid, status, flags);
}
// End of defs
} // namespace tdm
