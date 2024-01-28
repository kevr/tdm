#include "sys.h"
#include <cstdio>
#include <fcntl.h>
#include <pwd.h>

namespace tdm {

struct passwd *Sys::getpwuid(uid_t uid) const { return ::getpwuid(uid); }
int Sys::fcntl(int fd, int cmd, int arg) const { return ::fcntl(fd, cmd, arg); }
int Sys::pipe(int *pipedes) const { return ::pipe(pipedes); }

FILE *Sys::fdopen(int fd, const char *modes) const
{
    return ::fdopen(fd, modes);
}

FILE *Sys::popen(const char *cmd, const char *modes) const
{
    return ::popen(cmd, modes);
}

int Sys::read(int fd, void *buf, size_t nbytes) const
{
    return ::read(fd, buf, nbytes);
}

}; // namespace tdm
