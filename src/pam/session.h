// SPDX-License-Identifier: MIT
#ifndef PAM_SESSION_H
#define PAM_SESSION_H

#include "../lib/pam.h"
#include "../sys/passwd.h"
#include "../util/logger.h"
#include <string>

namespace tdm::pam {

class Session
{
  private:
    // /etc/pam.d/login
    static constexpr const char *_PAM_SERVICE = "tdm-login";

  private:
    Logger logger{"PAM"};

    struct pam_conv m_conv;
    pam_handle_t *m_pam = nullptr;

    User m_user;
    std::string m_password;

  public:
    Session(User user);
    ~Session(void);

  public:
    const std::string &user(void) const;
    int login(std::string password);
    int masquerade(void);
    void revert(void);

  private:
    static int authenticate_cb(int num_msg, const struct pam_message **msg,
                               struct pam_response **resp, void *appdata_ptr);
};

} // namespace tdm::pam

#endif /* PAM_SESSION_H */
