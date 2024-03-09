// SPDX-License-Identifier: MIT
#ifndef LIB_MOCKS_SYS_H
#define LIB_MOCKS_SYS_H

#include "../sys.h"
#include <gmock/gmock.h>

namespace tdm {

class MockSys : public Sys
{
  public:
    MOCK_METHOD(struct passwd *, getpwnam, (const char *), (override));
    MOCK_METHOD(struct passwd *, getpwuid, (uid_t), (override));
    MOCK_METHOD(int, execve, (const char *, char *const *, char *const *),
                (override));
    MOCK_METHOD(int, dup2, (int, int), (override));
    MOCK_METHOD(int, fcntl, (int, int, int), (override));
    MOCK_METHOD(pid_t, fork, (), (override));
    MOCK_METHOD(int, setegid, (gid_t), (override));
    MOCK_METHOD(int, seteuid, (uid_t), (override));
    MOCK_METHOD(int, pipe, (int *), (override));
    MOCK_METHOD(int, read, (int, void *, size_t), (override));
    MOCK_METHOD(int, waitpid, (pid_t, int *, int), (override));
};

} // namespace tdm

#endif /* LIB_MOCKS_SYS_H */
