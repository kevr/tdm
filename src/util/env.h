// SPDX-License-Identifier: MIT
#ifndef UTIL_ENV_H
#define UTIL_ENV_H

#include "../sys/passwd.h"
#include <string>

namespace tdm {

std::string getenv(const char *var, const std::string &def);
std::string xdg_data_home(const User &user);

} // namespace tdm

#endif /* UTIL_ENV_H */
