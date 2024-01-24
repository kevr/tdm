#include "sys.h"
#include <pwd.h>

namespace tdm {
struct passwd *Sys::getpwuid(uid_t uid) const { return ::getpwuid(uid); }
}; // namespace tdm
