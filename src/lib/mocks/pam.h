// SPDX-License-Identifier: MIT
#ifndef LIB_MOCKS_PAM_H
#define LIB_MOCKS_PAM_H

#include "../pam.h"
#include <gmock/gmock.h>

namespace tdm::lib {

class MockPam : public Pam
{
  public:
    MOCK_METHOD(int, start_confdir,
                (const char *, const char *, const struct pam_conv *,
                 char *confdir, pam_handle_t **),
                (const, override));
    MOCK_METHOD(int, authenticate, (pam_handle_t *, int), (const, override));
    MOCK_METHOD(int, open_session, (pam_handle_t *, int), (const, override));
    MOCK_METHOD(int, close_session, (pam_handle_t *, int), (const, override));
    MOCK_METHOD(int, end, (pam_handle_t *, int), (const, override));
};

} // namespace tdm::lib

#endif /* LIB_MOCKS_PAM_H */
