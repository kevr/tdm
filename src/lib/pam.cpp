#include "pam.h"

namespace tdm::lib {

int Pam::start_confdir(const char *service_name, const char *user,
                       const struct pam_conv *pam_conversion, char *confdir,
                       pam_handle_t **pamh)
{
    return pam_start_confdir(service_name, user, pam_conversion, confdir, pamh);
}

int Pam::authenticate(pam_handle_t *pamh, int flags)
{
    return pam_authenticate(pamh, flags);
}

int Pam::open_session(pam_handle_t *pamh, int flags)
{
    return pam_open_session(pamh, flags);
}

int Pam::close_session(pam_handle_t *pamh, int flags)
{
    return pam_close_session(pamh, flags);
}

int Pam::end(pam_handle_t *pamh, int pam_status)
{
    return pam_end(pamh, pam_status);
}

} // namespace tdm::lib
