// SPDX-License-Identifier: MIT
#ifndef LIB_PAM_H
#define LIB_PAM_H

#include "../singleton.h"
#include <security/pam_appl.h>

namespace tdm::lib {

class Pam
{
  public:
    virtual ~Pam(void) = default;
    virtual int start_confdir(const char *service_name, const char *user,
                              const struct pam_conv *pam_conversion,
                              char *confdir, pam_handle_t **pamh);
    virtual int authenticate(pam_handle_t *pamh, int flags);
    virtual int open_session(pam_handle_t *pamh, int flags);
    virtual int close_session(pam_handle_t *pamh, int flags);
    virtual int end(pam_handle_t *pamh, int pam_status);
};

inline Singleton<Pam> pam;

} // namespace tdm::lib

#endif /* LIB_PAM_H */
