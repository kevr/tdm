// SPDX-License-Identifier: MIT
#ifndef LIB_MOCKS_SYS_H
#define LIB_MOCKS_SYS_H

#include "../sys.h"
#include <gmock/gmock.h>

namespace tdm {
class MockSys : public Sys
{
  public:
    MOCK_METHOD(struct passwd *, getpwuid, (uid_t), (const, override));
    MOCK_METHOD(int, execve, (const char *, char *const *, char *const *),
                (const, override));
    MOCK_METHOD(int, dup2, (int, int), (const, override));
    MOCK_METHOD(int, fcntl, (int, int, int), (const, override));
    MOCK_METHOD(pid_t, fork, (), (const, override));
    MOCK_METHOD(int, pipe, (int *), (const, override));
    MOCK_METHOD(int, read, (int, void *, size_t), (const, override));
    MOCK_METHOD(int, waitpid, (pid_t, int *, int), (const, override));
};
} // namespace tdm

#endif /* LIB_MOCKS_SYS_H */
