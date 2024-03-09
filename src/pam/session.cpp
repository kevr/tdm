// SPDX-License-Identifier: MIT
#include "session.h"
#include "../lib/sys.h"
#include "../sys/passwd.h"
#include "../util/env.h"
#include <security/pam_appl.h>

namespace tdm::pam {

Session::Session(User user) : m_user(std::move(user))
{
}

Session::~Session(void)
{
    using lib::pam;

    if (m_pam) {
        pam->close_session(m_pam, 0);
        pam->end(m_pam, PAM_SUCCESS);
        revert();
    }
}

const std::string &Session::user(void) const
{
    return m_user.name();
}

int Session::login(std::string password)
{
    using lib::pam;

    try {
        m_user.populate();
    } catch (std::invalid_argument &exc) {
        logger.error("{}", exc.what());
        return -1;
    }
    m_password = std::move(password);

    m_conv.appdata_ptr = &*this;
    m_conv.conv = this->authenticate_cb;

    logger.info("authenticating user: '{}'", m_user.name());
    auto confdir = tdm::getenv("PAM_CONFDIR", "/etc/pam.d");
    int rc = pam->start_confdir(_PAM_SERVICE, m_user.name().c_str(), &m_conv,
                                confdir.data(), &m_pam);
    if (rc != PAM_SUCCESS) {
        logger.error("pam_start failed, error: {}", pam_strerror(m_pam, rc));
        return rc;
    }

    if (int rc = pam->authenticate(m_pam, 0); rc != PAM_SUCCESS) {
        logger.error("pam_authenticate failed, error: {}",
                     pam_strerror(m_pam, rc));
        return rc;
    }
    logger.info("successfully authenticated as '{}'", m_user.name());

    if (int rc = pam->open_session(m_pam, 0); rc != PAM_SUCCESS) {
        logger.error("pam_open_session failed, error: {}",
                     pam_strerror(m_pam, rc));
        return rc;
    }
    logger.info("opened session for '{}'", m_user.name());

    if (int rc = masquerade(); rc)
        return rc;

    return PAM_SUCCESS;
}

int Session::masquerade(void)
{
    if (int rc = lib::sys->seteuid(m_user.uid()); rc) {
        logger.error("unable to seteuid, error: {}", strerror(errno));
        return rc;
    }
    lib::sys->setegid(m_user.gid());

    logger.debug("euid = {}", geteuid());
    logger.debug("egid = {}", getegid());
    logger.debug("uid = {}", getuid());
    logger.debug("gid = {}", getgid());

    return 0;
}

void Session::revert(void)
{
    seteuid(getuid());
    setegid(getgid());
}

int Session::authenticate_cb(int, const struct pam_message **,
                             struct pam_response **resp, void *appdata_ptr)
{
    Session *auth = reinterpret_cast<Session *>(appdata_ptr);

    *resp = static_cast<struct pam_response *>(malloc(sizeof(pam_response)));
    struct pam_response *r = *resp;

    r->resp = static_cast<char *>(malloc(2048));
    strcpy(r->resp, auth->m_password.c_str());

    return 0;
}

} // namespace tdm::pam
