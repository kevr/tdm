#ifndef LIB_MOCKS_SYS_H
#define LIB_MOCKS_SYS_H

#include "../sys.h"
#include <gmock/gmock.h>

namespace tdm {
class MockSys : public Sys
{
  public:
    MOCK_METHOD(struct passwd *, getpwuid, (uid_t), (const, override));
    MOCK_METHOD(int, fcntl, (int, int, int), (const, override));
    MOCK_METHOD(int, pipe, (int *), (const, override));
    MOCK_METHOD(FILE *, fdopen, (int, const char *), (const, override));
    MOCK_METHOD(FILE *, popen, (const char *, const char *), (const, override));
    MOCK_METHOD(int, read, (int, void *, size_t), (const, override));
};
} // namespace tdm

#endif /* LIB_MOCKS_SYS_H */
