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
};

inline Singleton<Sys> sys;
}; // namespace tdm

#endif /* LIB_SYS_H */
