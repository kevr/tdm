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

  public:
    virtual int start_confdir(const char *service_name, const char *user,
                              const struct pam_conv *pam_conversion,
                              char *confdir, pam_handle_t **pamh) const;
    virtual int authenticate(pam_handle_t *pamh, int flags) const;
    virtual int open_session(pam_handle_t *pamh, int flags) const;
    virtual int close_session(pam_handle_t *pamh, int flags) const;
    virtual int end(pam_handle_t *pamh, int pam_status) const;
};

inline Singleton<Pam> pam;

} // namespace tdm::lib

#endif /* LIB_PAM_H */
