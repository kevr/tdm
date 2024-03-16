// SPDX-License-Identifier: MIT
#include "../term.h"
#include <gmock/gmock.h>

namespace tdm::lib {

class MockTerm : public Term
{
  public:
    virtual ~MockTerm(void) = default;
    MOCK_METHOD(int, tigetnum, (const char *), (override));
};

} // namespace tdm::lib
