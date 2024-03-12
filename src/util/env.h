// SPDX-License-Identifier: MIT
#ifndef UTIL_ENV_H
#define UTIL_ENV_H

#include "../sys/passwd.h"
#include <string>

namespace tdm {

std::string getenv(const char *var, const std::string &def);
std::filesystem::path xdg_data_home(void);
std::string xdg_data_home(const User &user);
std::filesystem::path tdm_data_dir(void);
std::filesystem::path tdm_log_dir(void);

} // namespace tdm

#endif /* UTIL_ENV_H */
