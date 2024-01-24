#ifndef LIB_MOCKS_SYS_H
#define LIB_MOCKS_SYS_H

#include "../sys.h"
#include <gmock/gmock.h>

namespace tdm {
class MockSys : public Sys
{
  public:
    MOCK_METHOD(struct passwd *, getpwuid, (uid_t), (const, override));
};
}; // namespace tdm

#endif /* LIB_MOCKS_SYS_H */
